#ifndef IPCOM_ERR_H
#define IPCOM_ERR_H

/* System call succeeded. */
#define IPCOM_SUCCESS                  0

/* System call failed (general failure). */
#define IPCOM_ERR_FAILED              -1000


/*
 *===========================================================================
 *                         general failures
 *===========================================================================
 */

/* Operation failed because insufficient memory is available. */
#define IPCOM_ERR_NO_MEMORY           -1010

/* failed because (entry) not found. */
#define IPCOM_ERR_NOT_FOUND           -1011

/* failed because not supported. */
#define IPCOM_ERR_NOT_SUPPORTED       -1012

/* failed because permission denied. */
#define IPCOM_ERR_PERMISSION_DENIED   -1013


#endif