# Password Manager
This project is a simple application to interact with a text file of login information. To protect the file from being read by anyone, users choose a password that is used to encrypt the file with. This way the file can't be read unless you know the correct password.

## Usage
The program will continue to prompt the user for new commands until closed. Simply enter the keyword to select a command, for example - "add" to add a record

**Adding records** - Enter each data field when prompted, can leave blanks.

**Search records** - Enter a search term, any record containing the term will be returned with an index that can be used for further functions.

**Edit records** - Enter the index of a record, enter new values for fields when prompted. If no change is wanted, leave blank. Service name can't be edited.

**Delete records** - Enter the index of a record to delete.

**Save** - Manually save all changes to the file, can change the password used to encrypt the file at this time.
