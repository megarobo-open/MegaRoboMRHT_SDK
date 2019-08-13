#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vxi11_user.h"

#include <rpc/rpc.h>
#include "vxi11.h"


#ifdef __cplusplus
extern "C" {
#endif

#define	VXI11_CLIENT		CLIENT
#define	VXI11_LINK		Create_LinkResp

struct _VXI11_CLINK {
    VXI11_CLIENT *client;
    VXI11_LINK *link;
};

/*****************************************************************************
 * GENERAL NOTES
 *****************************************************************************

 * Global variables. Keep track of multiple links per client. We need this
 * because:
 * - we'd like the library to be able to cope with multiple links to a given
 *   client AND multiple links to multiple clients
 * - we'd like to just refer to a client/link ("clink") as a single
 *   entity from user land, we don't want to worry about different
 *   initialisation procedures, depending on whether it's an instrument
 *   with the same address or not
 */

struct _vxi11_client_t {
    struct _vxi11_client_t *next;
    char address[20];
    CLIENT *client_address;
    int link_count;
};

static struct _vxi11_client_t *VXI11_CLIENTS = NULL;

/* Internal function declarations. */
static int _vxi11_open_link(VXI11_CLINK * clink, const char *address,
                            char *device);
static int _vxi11_close_link(VXI11_CLINK * clink, const char *address);


int vxi11_lib_version(int *major, int *minor, int *revision)
{
    if(major) *major = LIBVXI11_MAJOR;
    if(minor) *minor = LIBVXI11_MINOR;
    if(revision) *revision = LIBVXI11_REVISION;
    return LIBVXI11_VERSION_NUMBER;
}


/*****************************************************************************
 * KEY USER FUNCTIONS - USE THESE FROM YOUR PROGRAMS OR INSTRUMENT LIBRARIES *
 *****************************************************************************/

/* OPEN FUNCTIONS *
 * ============== */

/* Use this function from user land to open a device and create a link. Can be
 * used multiple times for the same device (the library will keep track).*/
int vxi11_open_device(VXI11_CLINK **clink, const char *address, char *device)
{
    int ret;
    struct _vxi11_client_t *tail, *client = NULL;
    char default_device[6] = "inst0";
    char *use_device;

    if(device){
        use_device = device;
    }else{
        use_device = default_device;
    }

    *clink = (VXI11_CLINK *) calloc(1, sizeof(VXI11_CLINK));
    if (!(*clink)) {
        return 1;
    }

    tail = VXI11_CLIENTS;
    while (tail) {
        if (strcmp(address, tail->address) == 0) {
            client = tail;
            break;
        }
        tail = tail->next;
    }

    if (!client) {
        client = (struct _vxi11_client_t *)calloc(1, sizeof(struct _vxi11_client_t));
        if (!client) {
            free(*clink);
            *clink = NULL;
            return 1;
        }

        (*clink)->client =
                clnt_create(address, DEVICE_CORE, DEVICE_CORE_VERSION,
                            "tcp");

        if ((*clink)->client == NULL) {
            clnt_pcreateerror(address);
            free(client);
            free(*clink);
            *clink = NULL;
            return 1;
        }
        ret = _vxi11_open_link(*clink, address, use_device);
        if (ret != 0) {
            clnt_destroy((*clink)->client);
            free(client);
            free(*clink);
            *clink = NULL;
            return 1;
        }

        strncpy(client->address, address, 20);
        client->client_address = (*clink)->client;
        client->link_count = 1;
        client->next = VXI11_CLIENTS;
        VXI11_CLIENTS = client;
    } else {
        /* Copy the client pointer address. Just establish a new link
         *  not a new client). Add one to the link count */
        (*clink)->client = client->client_address;
        ret = _vxi11_open_link((*clink), address, use_device);
        client->link_count++;
    }
    return 0;
}

/* CLOSE FUNCTION *
 * ============== */

/* Use this function from user land to close a device and/or sever a link. Can
 * be used multiple times for the same device (the library will keep track).*/
int vxi11_close_device(VXI11_CLINK * clink, const char *address)
{
    int ret = 0;

    struct _vxi11_client_t *tail, *last = NULL, *client = NULL;

    tail = VXI11_CLIENTS;
    while (tail) {
        if (strncmp(address, tail->address, 20) == 0) {
            client = tail;
            break;
        }
        last = tail;
        tail = tail->next;
    }

    if (!client) {
        ret = -4;
    } else {
        if (client->link_count > 1) {
            ret = _vxi11_close_link(clink, address);
            client->link_count--;
        }
        else {
            ret = _vxi11_close_link(clink, address);
            clnt_destroy(clink->client);

            if (last) {
                last->next = client->next;
            } else {
                VXI11_CLIENTS = client->next;
            }
        }
    }
    free(clink);
    return ret;
}

static char VXI_SEND_BUFF[MAX_SEND_SIZE];
int vxi11_send(VXI11_CLINK * clink, const char *cmd, size_t len)
{
    if(len > MAX_SEND_SIZE) {
        return -VXI11_ERROR_PARAMETER_ERROR;
    }
    if(len == 0) {
        return 0;
    }

    Device_WriteParms write_parms;
    memset(&write_parms, 0, sizeof(Device_WriteParms));
    memset(VXI_SEND_BUFF,'\0',MAX_SEND_SIZE);
    memcpy(VXI_SEND_BUFF, cmd, len);

    write_parms.lid = clink->link->lid;
    write_parms.io_timeout = VXI11_DEFAULT_TIMEOUT;
    write_parms.lock_timeout = 0;

    //flag==8表示数据发送结束，否则表示还没有完全发送完成
    write_parms.flags = 8; // 8 or 0
    write_parms.data.data_val = VXI_SEND_BUFF;
    write_parms.data.data_len = len;

    Device_WriteResp *write_resp = device_write_1(&write_parms, clink->client);
    if (write_resp == (Device_WriteResp *) NULL) {
        return -VXI11_NULL_WRITE_RESP;
    }

    int t_error = write_resp->error;
    if (t_error != 0) {
        return -(t_error);
    }

    return write_resp->size;
}

ssize_t vxi11_receive(VXI11_CLINK *clink, char *buffer, size_t len)
{
    return vxi11_receive_timeout(clink, buffer, len, VXI11_READ_TIMEOUT);
}

#define RCV_END_BIT	0x04	// An end indicator has been read
#define RCV_CHR_BIT	0x02	// A termchr is set in flags and a character which matches termChar is transferred
#define RCV_REQCNT_BIT	0x01	// requestSize bytes have been transferred.  This includes a request size of zero.

ssize_t vxi11_receive_timeout(VXI11_CLINK *clink, char *buffer, size_t len,
                              unsigned long timeout)
{
    if(len == 0) {
        return 0;
    }

    Device_ReadParms read_parms;
    memset(&read_parms, 0, sizeof(Device_ReadParms));

    read_parms.lid = clink->link->lid;
    read_parms.requestSize = (len < MAX_RECV_SIZE ? len : MAX_RECV_SIZE);
    read_parms.io_timeout = timeout;	/* in ms */
    read_parms.lock_timeout = 0;
    read_parms.flags = 0;
    read_parms.termChar = '\n';

    Device_ReadResp *read_resp;
//    read_resp->data.data_val = VXI_RECV_BUFF;

    read_resp = device_read_1(&read_parms, clink->client);
    if (read_resp == (Device_ReadResp *)NULL) {
        return -VXI11_NULL_READ_RESP;
    }
    if (read_resp->error != 0) {
        return -(read_resp->error);
    }

    if (read_resp->reason & RCV_REQCNT_BIT) {
        //所有请求数据全部接收完成
    }
    else if (read_resp->reason & RCV_END_BIT) {
        //实际返回的数据量小于请求的数据量
    }

    memcpy(buffer, read_resp->data.data_val, read_resp->data.data_len);
    return read_resp->data.data_len;
}

static int _vxi11_open_link(VXI11_CLINK * clink, const char *address,
                            char *device)
{
    Create_LinkParms link_parms;

    /* Set link parameters */
    link_parms.clientId = (long)clink->client;
    link_parms.lockDevice = 0;
    link_parms.lock_timeout = VXI11_DEFAULT_TIMEOUT;
    link_parms.device = device;

    clink->link = (Create_LinkResp *) calloc(1, sizeof(Create_LinkResp));

    clink->link = create_link_1(&link_parms, clink->client);

    if (clink->link == (Create_LinkResp *) NULL) {
        clnt_perror(clink->client, address);
        return -2;
    }
    return 0;
}

/* CLOSE FUNCTIONS *
 * =============== */
static int _vxi11_close_link(VXI11_CLINK * clink, const char *address)
{
    Device_Error *dev_error;
    dev_error = destroy_link_1(&clink->link->lid, clink->client);
    if (dev_error == (Device_Error *) NULL){
        clnt_perror(clink->client, address);
        return -1;
    }
    return 0;
}


#ifdef __cplusplus
}
#endif
