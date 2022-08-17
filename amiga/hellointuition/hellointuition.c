/***************************************************************************//**
 * 
 * \brief Main
 *
 * \file hellointuition.c
 *
 ******************************************************************************/
#include <clib/exec_protos.h>
#include <clib/intuition_protos.h>
#include <clib/dos_protos.h>
#include <intuition/intuitionbase.h>
#include <stdio.h>

// The "intuition.library"
struct IntuitionBase* IntuitionBase = NULL;

// The text for CLI and window title
static const char titleAndVersion[] = "Hello Intuition";

//! Base error value
#define ERR_BASE 0

//! Success return value
static const int ERR_NONE = ERR_BASE;

//! Some, but known error
static const int ERR_KNOWN = ERR_BASE - 1;

//! Close all
static const int ERR_CLOSE = ERR_BASE - 2;

/***************************************************************************//**
*  
* \brief NDK13 compatibility function. Prior to Release 2 (V36), there were 
*        no public screens so e.g. the method 'LockPubScreen()' cannot be used.
*        This method searches the list of all screens and locate the
*        default Workbench screen.
* \return The pointer to the Workbench Screen structure or NULL
*
******************************************************************************/
struct Screen* getWorkbenchPubScreen();

/***************************************************************************//**
*  
* \brief Program main entry
* \return ERR_NONE if everything went successfully or teh corresponding error 
*         error code
*
******************************************************************************/
int main()
{
    printf ( "Program started!\n" );
    int err = ERR_KNOWN;

    IntuitionBase = (struct IntuitionBase*) OpenLibrary( "intuition.library", 0 );
    
    if ( IntuitionBase )
    {
        printf( titleAndVersion );
        printf( "\n" );

        struct Screen* workBenchScreen = getWorkbenchPubScreen();
        if ( workBenchScreen )
        {
            struct NewWindow newWindow;
            newWindow.LeftEdge = 0;
            newWindow.TopEdge = 0 ;

            // The WA_InnerWidth and WA_InnerHeight are missing, so the size needs to be computed manually
            newWindow.Width = 400 + workBenchScreen->WBorLeft + workBenchScreen->WBorRight;
            newWindow.Height = 100 + workBenchScreen->WBorTop + workBenchScreen->WBorBottom + workBenchScreen->RastPort.TxHeight + 1;
            newWindow.DetailPen = -1;
            newWindow.BlockPen = -1;
            newWindow.IDCMPFlags =  CLOSEWINDOW | RAWKEY ;
            newWindow.Flags = WINDOWCLOSE | WINDOWDEPTH | WINDOWDRAG | WINDOWACTIVE | GIMMEZEROZERO | ACTIVATE;
            newWindow.FirstGadget = NULL;
            newWindow.CheckMark = NULL;
            newWindow.Title = (UBYTE*) titleAndVersion;
            newWindow.Screen = NULL; // Used only if we have defined a CUSTOMSCREEN 
            newWindow.BitMap = NULL;
            newWindow.Type = WBENCHSCREEN;


            // Create the window on the Workbench public screen 
            struct Window* window = OpenWindow( &newWindow );
            if ( window )
            {
                // Wait loop
                BOOL loop = TRUE;

                while ( loop )
                {
                    printf( "Wait for signal 'SIGBREAKF_CTRL_C' or message\n" );
                    
                    // Message port signal mask
                    ULONG msgSignalMask = (ULONG) 1 << window->UserPort->mp_SigBit;

                    // Put task to 'waiting' stare and wait for CTRL+C signal mask or message
                    ULONG signals = Wait( SIGBREAKF_CTRL_C | msgSignalMask );

                    // CTRL+C closes the app
                    if ( signals & SIGBREAKF_CTRL_C ) 
                    {
                        printf( "Signal 'SIGBREAKF_CTRL_C' received\n" );
                        loop = FALSE;
                        err = ERR_CLOSE;
                    }

                    // Check if there is a message
                    struct IntuiMessage* im = NULL;
                    if ( signals & msgSignalMask )
                    {
                        printf( "Signal 'Message Port' received\n" );

                        // Get the message
                        im = (struct IntuiMessage*) GetMsg( window->UserPort );
                        if ( im )
                        {
                            // If close icon was pressed close the app
                            if ( im->Class == CLOSEWINDOW )
                            {
                                printf( "Message 'CLOSEWINDOW' received\n" );
                                loop = FALSE;
                                err = ERR_CLOSE;
                            }
                            // Inform sender that the message was processed
                            ReplyMsg( (struct Message*) im );
                        }
                    } 
                }
                CloseWindow( window );
                window  = NULL;
            }
            else
            {
                printf( "[ERROR] Window cannot be opened\n" );
                err = ERR_KNOWN;
            }
        }
        else
        {
            printf( "[ERROR] Workbench screen cannot be accessed\n" );
            err = ERR_KNOWN;           
        }
    }
    else
    {
        printf ( "[ERROR] The 'intuition.library' cannot be opened\n" );
        err = ERR_KNOWN;
    };

    CloseLibrary( (struct Library*) IntuitionBase );

    printf( "Good Bye!\n"  );
    return err;
}

/***************************************************************************//**
*  
* \brief NDK13 compatibility function. Prior to Release 2 (V36), there were 
*        no public screens so e.g. the method 'LockPubScreen()' cannot be used.
*        This method searches the list of all screens and locate the
*        default Workbench screen.
* \return The pointer to the Workbench Screen structure or NULL
*
******************************************************************************/
struct Screen* getWorkbenchPubScreen()
{
    // Prior to Release 2 (V36), there were no public screens,     
    // just Workbench. In those older systems, windows can be     
    // opened on Workbench without locking or a pointer by setting 
    // the Type=WBENCHSCREEN in struct NewWindow.

    struct Screen* s = IntuitionBase->FirstScreen;
    while ( s )
    {
        if ( ( s->Flags & SCREENTYPE ) == WBENCHSCREEN )       
        {
            printf( "Screen '%s' is the Workbench one\n", s->Title );
            break;
        }
        else
        {
            s = s->NextScreen;
        }
    }

    return s;
}