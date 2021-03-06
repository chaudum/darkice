/*------------------------------------------------------------------------------

   Copyright (c) 2000 Tyrell Corporation. All rights reserved.

   Tyrell DarkIce

   File     : OssDspSource.h
   Version  : $Revision$
   Author   : $Author$
   Location : $Source$
   
   Copyright notice:

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License  
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.
   
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of 
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
    GNU General Public License for more details.
   
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

------------------------------------------------------------------------------*/
#ifndef OSS_DSP_SOURCE_H
#define OSS_DSP_SOURCE_H

#ifndef __cplusplus
#error This is a C++ include file
#endif


/* ============================================================ include files */

#include "Reporter.h"
#include "AudioSource.h"


/* ================================================================ constants */


/* =================================================================== macros */


/* =============================================================== data types */

/**
 *  An audio input based on /dev/dsp-like raw devices
 *
 *  @author  $Author$
 *  @version $Revision$
 */
class OssDspSource : public AudioSource, public virtual Reporter
{
    private:

        /**
         *  The file name of the OSS DSP device (e.g. /dev/dsp or /dev/dsp0).
         */
        char      * fileName;

        /**
         *  The low-level file descriptor of the OSS DSP device.
         */
        int         fileDescriptor;

        /**
         *  Indicates wether the low-level OSS DSP device is in a recording
         *  state.
         */
        bool        running;


    protected:

        /**
         *  Default constructor. Always throws an Exception.
         *
         *  @exception Exception
         */
        inline
        OssDspSource ( void )                       throw ( Exception )
        {
            throw Exception( __FILE__, __LINE__);
        }

        /**
         *  Initialize the object
         *
         *  @param name the file name of the OSS DSP device.
         *  @exception Exception
         */
        void
        init (  const char    * name )              throw ( Exception );

        /**
         *  De-iitialize the object
         *
         *  @exception Exception
         */
        void
        strip ( void )                              throw ( Exception );


    public:

        /**
         *  Constructor.
         *
         *  @param name the file name of the OSS DSP device
         *              (e.g. /dev/dsp or /dev/dsp0).
         *  @param sampleRate samples per second (e.g. 44100 for 44.1kHz).
         *  @param bitsPerSample bits per sample (e.g. 16 bits).
         *  @param channel number of channels of the audio source
         *                 (e.g. 1 for mono, 2 for stereo, etc.).
         *  @exception Exception
         */
        inline
        OssDspSource (  const char    * name,
                        int             sampleRate    = 44100,
                        int             bitsPerSample = 16,
                        int             channel       = 2 )
                                                        throw ( Exception )

                    : AudioSource( sampleRate, bitsPerSample, channel)
        {
            init( name);
        }

        /**
         *  Copy Constructor.
         *
         *  @param source the object to copy.
         *  @exception Exception
         */
        inline
        OssDspSource (  const OssDspSource &    ds )    throw ( Exception )
                    : AudioSource( ds )
        {
            init( ds.fileName);
        }

        /**
         *  Destructor.
         *
         *  @exception Exception
         */
        inline virtual
        ~OssDspSource ( void )                          throw ( Exception )
        {
            strip();
        }

        /**
         *  Assignment operator.
         *
         *  @param ds the object to assign to this one.
         *  @return a reference to this object.
         *  @exception Exception
         */
        inline virtual OssDspSource &
        operator= (     const OssDspSource &     ds )   throw ( Exception )
        {
            if ( this != &ds ) {
                strip();
                AudioSource::operator=( ds);
                init( ds.fileName);
            }
            return *this;
        }

        /**
         *  Tell if the data from this source comes in big or little endian.
         *
         *  @return true if the source is big endian, false otherwise
         */
        virtual bool
        isBigEndian ( void ) const           throw ();

        /**
         *  Open the OssDspSource.
         *  This does not put the OSS DSP device into recording mode.
         *  To start getting samples, call either canRead() or read().
         *
         *  @return true if opening was successful, false otherwise
         *  @exception Exception
         *  
         *  @see #canRead
         *  @see #read
         */
        virtual bool
        open ( void )                                   throw ( Exception );

        /**
         *  Check if the OssDspSource is open.
         *
         *  @return true if the OssDspSource is open, false otherwise.
         */
        inline virtual bool
        isOpen ( void ) const                           throw ()
        {
            return fileDescriptor != 0;
        }

        /**
         *  Check if the OssDspSource can be read from.
         *  Blocks until the specified time for data to be available.
         *  Puts the OSS DSP device into recording mode.
         *
         *  @param sec the maximum seconds to block.
         *  @param usec micro seconds to block after the full seconds.
         *  @return true if the OssDspSource is ready to be read from,
         *          false otherwise.
         *  @exception Exception
         */
        virtual bool
        canRead (               unsigned int    sec,
                                unsigned int    usec )  throw ( Exception );

        /**
         *  Read from the OssDspSource.
         *  Puts the OSS DSP device into recording mode.
         *
         *  @param buf the buffer to read into.
         *  @param len the number of bytes to read into buf
         *  @return the number of bytes read (may be less than len).
         *  @exception Exception
         */
        virtual unsigned int
        read (                  void          * buf,
                                unsigned int    len )   throw ( Exception );

        /**
         *  Close the OssDspSource.
         *
         *  @exception Exception
         */
        virtual void
        close ( void )                                  throw ( Exception );
};


/* ================================================= external data structures */


/* ====================================================== function prototypes */



#endif  /* OSS_DSP_SOURCE_H */


/*------------------------------------------------------------------------------
 
  $Source$

  $Log$
  Revision 1.6  2002/12/20 10:40:40  darkeye
  added support for big endian OSS devices (like Linux PowerPC)

  Revision 1.5  2001/09/18 14:57:19  darkeye
  finalized Solaris port

  Revision 1.4  2001/09/02 14:08:40  darkeye
  setting the sound card recording sample rate is now more relaxed
  there is no error reported if the sample rate is not exactly the same

  Revision 1.3  2000/11/12 13:31:40  darkeye
  added kdoc-style documentation comments

  Revision 1.2  2000/11/05 17:37:24  darkeye
  removed clone() functions

  Revision 1.1.1.1  2000/11/05 10:05:53  darkeye
  initial version

  
------------------------------------------------------------------------------*/

