#ifndef	_VXI11_USER_H_
#define	_VXI11_USER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#define LIBVXI11_MAJOR 2
#define LIBVXI11_MINOR 0
#define LIBVXI11_REVISION 0
/* LIBVXI11_VERSION_NUMBER looks like 2002001 for e.g. version 2.2.1. */
#define LIBVXI11_VERSION_NUMBER (LIBVXI11_MAJOR*1000000+LIBVXI11_MINOR*1000+LIBVXI11_REVISION)

typedef	struct _VXI11_CLINK VXI11_CLINK;

/* Default timeout value to use, in ms. */
#define	VXI11_DEFAULT_TIMEOUT	2000

/* Read timeout to use, in ms. */
#define	VXI11_READ_TIMEOUT	2000

/* vxi11_receive() return value if a query times out ON THE INSTRUMENT (and so
 * we have to resend the query). */
#define	VXI11_NULL_READ_RESP	50

/* vxi11_send() return value if a sent command times out ON THE INSTRUMENT. */
#define	VXI11_NULL_WRITE_RESP	51


#define VXI11_ERROR_NO_ERROR                            (0)
#define VXI11_ERROR_SYNTAX_ERROR                        (1)
#define VXI11_ERROR_DEVICE_NOT_ACCESSIBLE               (3)
#define VXI11_ERROR_INVALID_LINK_IDENTIFIER             (4)
#define VXI11_ERROR_PARAMETER_ERROR                     (5)
#define VXI11_ERROR_CHANNEL_NOT_ESTABLISHED             (6)
#define VXI11_ERROR_OPERATION_NOT_SUPPORTED             (8)
#define VXI11_ERROR_OUT_OF_RESOURCES                    (9)
#define VXI11_ERROR_DEVICE_LOCKED_BY_ANOTHER_LINK       (11)
#define VXI11_ERROR_NO_LOCK_HELD_BY_THIS_LINK           (12)
#define VXI11_ERROR_IO_TIMEOUT                          (15)
#define VXI11_ERROR_IO_ERROR                            (17)
#define VXI11_ERROR_INVALID_ADDRESS                     (21)
#define VXI11_ERROR_ABORT                               (23)
#define VXI11_ERROR_CHANNEL_ALREADY_ESTABLISHED         (29)

#define MAX_SEND_SIZE 1024
#define MAX_RECV_SIZE 1024


/* Function: vxi11_library_version
 *
 * Return the version of the current library.
 *
 * Parameters:
 *  major -    an integer pointer. If not NULL, the major version of the
 *             library will be returned in this variable.
 *  minor -    an integer pointer. If not NULL, the minor version of the
 *             library will be returned in this variable.
 *  revision - an integer pointer. If not NULL, the revision of the library will
 *             be returned in this variable.
 *
 * Returns:
 *  LIBVXI11_VERSION_NUMBER, which is a unique number based on the major,
 *      minor and revision values.
 */
int vxi11_lib_version(int *major, int *minor, int *revision);


/* Function: vxi11_open_device
 *
 * Open a connection to an instrument.
 *
 * Parameters:
 *  clink   - pointer to a VXI11_CLINK pointer, will be initialised on a
 *            successful connection.
 *  address - the IP address or (where supported) USB address for the
 *            instrument to connect to.
 *  device   - some instruments have multiple interfaces, this allows you to
 *            specify which to connect to. Set to NULL to use the default of
 *            "inst0".
 *
 * Returns:
 *  0 - on success
 *  1 - on failure. clink will not be a valid pointer.
 */
int vxi11_open_device(VXI11_CLINK **clink, const char *address, char *device);


/* Function: vxi11_close_device
 *
 * Parameters:
 *  clink   - a valid VXI11_CLINK pointer.
 *  address - the IP address or (where supported) USB address for the
 *            instrument.
 *
 * Returns:
 *  0 - on success
 */
int vxi11_close_device(VXI11_CLINK *clink, const char *address);


/* Function: vxi11_send
 *
 * Send data to an instrument.
 *
 * Parameters:
 *  clink - a valid VXI11_CLINK pointer.
 *  cmd   - the command to send as an array of bytes
 *  len   - the length of cmd
 *
 * Returns:
 *  >0  on success
 */
int vxi11_send(VXI11_CLINK *clink, const char *cmd, size_t len);


ssize_t vxi11_receive(VXI11_CLINK *clink, char *buffer, size_t len);


/* Function: vxi11_receive_timeout
 *
 * Receive data from an instrument. Own timeout can be specified.
 *
 * Parameters:
 *  clink   - a valid VXI11_CLINK pointer.
 *  buffer  - valid memory location in which to receive data.
 *  len     - number of bytes requested - buffer must be at least this large
 *  timeout - the number of milliseconds to wait before returning if no data is
 *            received.
 *
 * Returns:
 *  >0  on success
 */
ssize_t vxi11_receive_timeout(VXI11_CLINK *clink, char *buffer, size_t len, unsigned long timeout);

#ifdef __cplusplus
}
#endif

#endif
