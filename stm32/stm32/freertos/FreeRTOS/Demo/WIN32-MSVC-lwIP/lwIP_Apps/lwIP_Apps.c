/*
    FreeRTOS V7.4.0 - Copyright (C) 2013 Real Time Engineers Ltd.

    FEATURES AND PORTS ARE ADDED TO FREERTOS ALL THE TIME.  PLEASE VISIT
    http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS tutorial books are available in pdf and paperback.        *
     *    Complete, revised, and edited pdf reference manuals are also       *
     *    available.                                                         *
     *                                                                       *
     *    Purchasing FreeRTOS documentation will not only help you, by       *
     *    ensuring you get running as quickly as possible and with an        *
     *    in-depth knowledge of how to use FreeRTOS, it will also help       *
     *    the FreeRTOS project to continue with its mission of providing     *
     *    professional grade, cross platform, de facto standard solutions    *
     *    for microcontrollers - completely free of charge!                  *
     *                                                                       *
     *    >>> See http://www.FreeRTOS.org/Documentation for details. <<<     *
     *                                                                       *
     *    Thank you for using FreeRTOS, and thank you for your support!      *
     *                                                                       *
    ***************************************************************************


    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.

    >>>>>>NOTE<<<<<< The modification to the GPL is included to allow you to
    distribute a combined work that includes FreeRTOS without being obliged to
    provide the source code for proprietary components outside of the FreeRTOS
    kernel.

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
    details. You should have received a copy of the GNU General Public License
    and the FreeRTOS license exception along with FreeRTOS; if not itcan be
    viewed here: http://www.freertos.org/a00114.html and also obtained by
    writing to Real Time Engineers Ltd., contact details for whom are available
    on the FreeRTOS WEB site.

    1 tab == 4 spaces!

    ***************************************************************************
     *                                                                       *
     *    Having a problem?  Start by reading the FAQ "My application does   *
     *    not run, what could be wrong?"                                     *
     *                                                                       *
     *    http://www.FreeRTOS.org/FAQHelp.html                               *
     *                                                                       *
    ***************************************************************************


    http://www.FreeRTOS.org - Documentation, books, training, latest versions,
    license and Real Time Engineers Ltd. contact details.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, and our new
    fully thread aware and reentrant UDP/IP stack.

    http://www.OpenRTOS.com - Real Time Engineers ltd license FreeRTOS to High
    Integrity Systems, who sell the code with commercial support,
    indemnification and middleware, under the OpenRTOS brand.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.
*/

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* lwIP core includes */
#include "lwip/opt.h"
#include "lwip/tcpip.h"

/* applications includes */
#include "apps/httpserver_raw_from_lwIP_download/httpd.h"

/* include the port-dependent configuration */
#include "lwipcfg_msvc.h"

/* Dimensions the cTxBuffer array - which is itself used to hold replies from
command line commands.  cTxBuffer is a shared buffer, so protected by the
xTxBufferMutex mutex. */
#define lwipappsTX_BUFFER_SIZE	1024

/* The maximum time to block waiting to obtain the xTxBufferMutex to become
available. */
#define lwipappsMAX_TIME_TO_WAIT_FOR_TX_BUFFER_MS	( 100 / portTICK_PERIOD_MS )

/* Definitions of the various SSI callback functions within the pccSSITags
array.  If pccSSITags is updated, then these definitions must also be updated. */
#define ssiTASK_STATS_INDEX			0
#define ssiRUN_TIME_STATS_INDEX		1

/*-----------------------------------------------------------*/

/*
 * The function that implements the lwIP based sockets command interpreter
 * server.
 */
extern void vBasicSocketsCommandInterpreterTask( void *pvParameters );

/*
 * The SSI handler callback function passed to lwIP.
 */
static unsigned short uslwIPAppsSSIHandler( int iIndex, char *pcBuffer, int iBufferLength );

/*-----------------------------------------------------------*/

/* The SSI strings that are embedded in the served html files.  If this array
is changed, then the index position defined by the #defines such as
ssiTASK_STATS_INDEX above must also be updated. */
static const char *pccSSITags[] =
{
	"rtos_stats",
	"run_stats"
};

/* Semaphore used to guard the Tx buffer. */
static SemaphoreHandle_t xTxBufferMutex = NULL;

/* The Tx buffer itself.  This is used to hold the text generated by the
execution of command line commands, and (hopefully) the execution of
server side include callbacks.  It is a shared buffer so protected by the
xTxBufferMutex mutex.  pcLwipAppsBlockingGetTxBuffer() and
vLwipAppsReleaseTxBuffer() are provided to obtain and release the
xTxBufferMutex respectively.  pcLwipAppsBlockingGetTxBuffer() must be used with
caution as it has the potential to block. */
static signed char cTxBuffer[ lwipappsTX_BUFFER_SIZE ];

/*-----------------------------------------------------------*/


/* Called from the TCP/IP thread. */
void lwIPAppsInit( void *pvArgument )
{
ip_addr_t xIPAddr, xNetMask, xGateway;
extern err_t ethernetif_init( struct netif *xNetIf );
static struct netif xNetIf;

	( void ) pvArgument;

	/* Set up the network interface. */
	ip_addr_set_zero( &xGateway );
	ip_addr_set_zero( &xIPAddr );
	ip_addr_set_zero( &xNetMask );

	LWIP_PORT_INIT_GW(&xGateway);
	LWIP_PORT_INIT_IPADDR(&xIPAddr);
	LWIP_PORT_INIT_NETMASK(&xNetMask);
	printf("Starting lwIP, local interface IP is %s\n", ip_ntoa(&xIPAddr));

	netif_set_default( netif_add( &xNetIf, &xIPAddr, &xNetMask, &xGateway, NULL, ethernetif_init, tcpip_input ) );
	netif_set_up( &xNetIf );

	/* Install the server side include handler. */
	http_set_ssi_handler( uslwIPAppsSSIHandler, pccSSITags, sizeof( pccSSITags ) / sizeof( char * ) );

	/* Create the mutex used to ensure mutual exclusive access to the Tx
	buffer. */
	xTxBufferMutex = xSemaphoreCreateMutex();
	configASSERT( xTxBufferMutex );

	/* Create the httpd server from the standard lwIP code.  This demonstrates
	use of the lwIP raw API. */
	httpd_init();

	/* Create the FreeRTOS defined basic command server.  This demonstrates use
	of the lwIP sockets API. */
	xTaskCreate( vBasicSocketsCommandInterpreterTask, "CmdInt", configMINIMAL_STACK_SIZE * 10, NULL, configMAX_PRIORITIES - 2, NULL );
}
/*-----------------------------------------------------------*/

static unsigned short uslwIPAppsSSIHandler( int iIndex, char *pcBuffer, int iBufferLength )
{
static unsigned int uiUpdateCount = 0;
static char cUpdateString[ 200 ];
extern char *pcMainGetTaskStatusMessage( void );

	/* Unused parameter. */
	( void ) iBufferLength;

	/* The SSI handler function that generates text depending on the index of
	the SSI tag encountered. */

	switch( iIndex )
	{
		case ssiTASK_STATS_INDEX :
			vTaskList( pcBuffer );
			break;

		case ssiRUN_TIME_STATS_INDEX :
			vTaskGetRunTimeStats( pcBuffer );
			break;
	}

	/* Include a count of the number of times an SSI function has been executed
	in the returned string. */
	uiUpdateCount++;
	sprintf( cUpdateString, "\r\n\r\n%u\r\nStatus - %s", uiUpdateCount, pcMainGetTaskStatusMessage() );
	strcat( pcBuffer, cUpdateString );

	return strlen( pcBuffer );
}
/*-----------------------------------------------------------*/

signed char *pcLwipAppsBlockingGetTxBuffer( void )
{
signed char *pcReturn;

	/* Attempt to obtain the semaphore that guards the Tx buffer. */
	if( xSemaphoreTakeRecursive( xTxBufferMutex, lwipappsMAX_TIME_TO_WAIT_FOR_TX_BUFFER_MS ) == pdFAIL )
	{
		/* The semaphore could not be obtained before timing out. */
		pcReturn = NULL;
	}
	else
	{
		/* The semaphore was obtained successfully.  Return a pointer to the
		Tx buffer. */
		pcReturn = cTxBuffer;
	}

	return pcReturn;
}
/*-----------------------------------------------------------*/

void vLwipAppsReleaseTxBuffer( void )
{
	/* Finished with the Tx buffer.  Return the mutex. */
	xSemaphoreGiveRecursive( xTxBufferMutex );
}



