Downloading Adobe Photoshop CS6 and Updates

Adobe Photoshop CS6 and its updates can be found in the Applications > Share forum. All files feature direct download links. All questions and support are answered in this topic.

Installing Adobe Photoshop CS6

The following instructions are for the standalone version only. This will not work for the bundled Adobe software. Note: If you have blocked Adobe by editing your hosts file before, remove the old addresses before installation. If you do not know what I am talking about, you don't need to worry about it.

How to install Adobe Photoshop CS6
Disconnect your computer's Internet connection.
Open Adobe Photoshop CS6 folder.
Open the Install package.
Select "Try" on the Adobe Welcome window.
If prompted, select "Connect Later".
Launch Photoshop
Quit Photoshop
Replace amtlib.framework in Application_Name.app/Contents/Frameworks/, as explained below.
Block all Adobe Internet connections, as explained below.
Restart your computer.
Connect to the Internet.
Enjoy!

How to replace amtlib.framework
Download amtlib.framework from any of the following links.

CODE: SELECT ALL
http://ge.tt/9Jy3yyu/v/0?c
http://mx8y37.pjointe.com/
http://www.mirrorupload.net/file/1RSKAHZA/#!amtlib.framework.zip
http://mirrorstack.com/o43jrdxec5wp

Double click amtlib.framework.zip to unzip file.
Open Applications folder.
Open Adobe Photoshop CS6 folder.
Right click on Adobe Photoshop CS6 application.
Select "Show Package Contents"
Open Contents folder.
Open Frameworks folder.
Replace amtlib.framework.

How to block Adobe Internet connections
Open Terminal application (located in /Applications/Utilities/).
Copy, paste, and enter the following code in Terminal.

CODE: SELECT ALL
sudo /Applications/TextEdit.app/Contents/MacOS/TextEdit /etc/hosts

Copy and paste the following lines to the hosts file.

CODE: SELECT ALL
127.0.0.1 lmlicenses.wip4.adobe.com
127.0.0.1 lm.licenses.adobe.com

Save and close the hosts file.
Copy, paste, and enter the following in Terminal.

CODE: SELECT ALL
sudo dscacheutil -flushcache


Installing Adobe Photoshop CS6 Updates

The latest updates can be downloaded in the Applications > Share forum. Installing updates from the application updater is not always recommended. In many cases, it can break your application. Please make sure to follow the above instructions carefully. Reply to this topic if you are unsure about any of the steps.

Now you're done!
