DamFix
============

This was written a while ago in order to move data repositories 
for NPS Telescope versions 6 and 7, for example in order to move 
assets to a larger store the database needs to be updated in
order for the files to be retieved without error and display
the correct freindly name also.

The purpose of the application is to alter the Telescope database 
to reflect changes in the physical asset location represented 
by filebroker location, filebroker name, disk label or server 
name within the DOC_RENDITIONS table for the FILE_LOCATION and 
LONG_NAME column fields.

There are five (5) sections within the dialog, File Location, 
Long Name, Root Folder, Server and Database. Each section will 
be covered seperatley.

File Location.
This section effect the FILE_LOCATION column within the DOC_RENDITIONS 
table of Telescope. The "Label" entries refer to Disk Label, and 
"FB" entries refer to File Broker name. The Old Label and Old FB 
are used for location of records, and are only updated if new 
entries are provided under  New Label or New FB. These can be 
updated independatley of each other if required. 
The FILE_LOCATION column is used by the filebroker to locate and 
download the asset to the user. If this field is incorrect the 
asset will be unavailable for download, only visable as metadata 
and thumbnail entry within Telescope. If assets need to be moved 
to a different physical location for space considerations, this 
field will have to be modified to enable assets to be available 
from the new location.

Long Name.
This section effect the LONG_NAME column within the DOC_RENDITIONS 
table of Telescope. The "Label" entries refer to Disk Label, and 
"FB" entries refer to File Broker name. The Old Label and Old FB 
are used for location of records, and are only updated if new 
entries are provided under New Label or New FB. These can be 
updated independatley of each other if required. The LONG_NAME 
column is used for visual presentation of the asset location to 
the end user, and will not have any effect on the availability 
of the asset if not set to the correct location. It will however 
cause confusion to some users and any IT staff using this as a 
reference, so keeping in sync with the correct location is advised.

Root Folder.
This is an optional field. It only needs to be used if a single 
top level folder under a filebroker is desired to be moved, and 
specifies the folder name. In the example image above only the 
Fall season tree would be moved. This is a special use condition 
that is not a normal operation. 

Server.
This is an optional field. It only is used to change the server 
of the file store that the asset resides on. Typically only used on the development system to reassign production assets.

Database.
Requred field. This dropdown will present a list of registered 
TNS names from the oracle tnsnames.ora file on the machine that 
is running the application. Selection of the correct database 
configuration is required, and also allows for updates to be run 
from a remote machine with appropiate configuration settings

