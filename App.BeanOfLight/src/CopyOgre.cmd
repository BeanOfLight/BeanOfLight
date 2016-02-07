@echo Copying Ogre Resources, configuration = %1

set Suffix=%1
set Configuration=%2

set Source=..\..\..\ThirdParty\OGRE-SDK-1.9.0-vc140-x86-09.01.2016
set OgreSource=OgreResource%Configuration%.%Suffix%.txt

set Target=..\..\Output.%Suffix%\%Configuration%\bin

for /F %%I in (%OgreSource%) do (
	copy %Source%\%%I %Target%
)

