# opendiameter
Open Diameter based on http://sourceforge.net/projects/diameter/files/c_server/1.0.7-i/opendiameter-1.0.7-i.tar.gz/download

Below will provide procedure on how compile OpenDiameter on Windows and Linux.

OpenDiameter is depeneded on ACE and BOOST.

1. Compile ACE on Windows
   1). Install Visual Studio 2015 RC if not
   
   2). Generate static project files
      This step can be finished on CYGWIN or Linux
      Download and extract ACE package to D:\ACE-6.3.2 
      
      CYGWIN:
        export ACE_ROOT=/cygdrive/d/ACE-6.3.2/ACE_wrappers
        export CIAO_ROOT=/cygdrive/d/ACE-6.3.2/ACE_wrappers
        export DANCE_ROOT=/cygdrive/d/ACE-6.3.2/ACE_wrappers

        ### Visual Studio 2015
        $ACE_ROOT/bin/mwc.pl -type vc14 -static ACE.mwc
        $ACE_ROOT/bin/mwc.pl -type vc14 -static -name_modifier "*_vc14_Static" -apply_project -exclude TAO/CIAO -recurse -hierarchy
   
   3). Edit D:\ACE-6.3.2\ACE_wrappers\ace\config.h as below:
/**
 * Static version of ACE libraries should be built with ACE_AS_STATIC_LIBS macro defined.
 * This macro should also be used in application projects that link to static ACE libraries

 * List out all the predefined macros available on the system.
 * For example: cpp -dM ./dummy.hxx
 *
 */

#define ACE_NO_INLINE
#define ACE_HAS_IPV6
#define ACE_USES_IPV4_IPV6_MIGRATION

#if defined (WIN32)
#include "ace/config-win32.h"
#endif

#if defined (linux)
#include "ace/config-linux.h"
#endif

#if defined (__sun)
#include "ace/config-sunos5.10.h"
#endif

   4). Open and build solution D:\ACE-6.3.2\ACE_wrappers\ACE_vc14_Static.sln
   
   5). Library from three projects are required: ACE_vc14_Static, ACEXML_vc14_Static and ACEXML_Parser_vc14_Static.
   
   6). Static library will be in D:\ACE-6.3.2\ACE_wrappers\lib: ACEsd.lib, ACEXML_Parsersd.lib, ACEXMLsd.lib

2. Compile BOOST on Windows
   1). Download and extract ACE package to D:\boost_1_58_0
   
   2). Open a VS2015 Dev command window
       cd D:\boost_1_58_0
       bootstrap.bat
       b2.exe stage toolset=msvc-14.0 link=static runtime-link=static

   3). Static library will be in D:\boost_1_58_0\lib
   

3. Compile Open Diameter on Windows
   1). Extract opendiameter to D:\opendiameter

   2). Open and build solution D:\opendiameter\VC14\OpenDiameter\OpenDiameter_Static.sln
       Change or copy ACE and BOOST static library to related directory.

   3). Static library will be in D:\opendiameter\VC14\lib

test1234