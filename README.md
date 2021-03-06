# Wire

This repository is part of the source code of Wire. You can find more information at [wire.com](https://wire.com) or by contacting opensource@wire.com.

You can find the published source code at [github.com/wireapp/wire](https://github.com/wireapp/wire). 

For licensing information, see the attached LICENSE file and the list of third-party licenses at [wire.com/legal/licenses/](https://wire.com/legal/licenses/).

## Building

Building for node is done with node-gyp:

`node-gyp configure`
`node-gyp build`

Building for electron seems to require the use of electron-rebuild:

`electron-rebuild -v <electron version>`

# Usage

how to use the module:
```
const addressBook = require('node-addressbook');

console.log("Number of Contacts: ", addressBook.getContactsCount());

console.log("Me: ", addressBook.getMe());

console.log("Contact [9]: ", addressBook.getContact(9));

console.log("Start Importing Contacts");

addressBook.getContacts(
    progress => console.log("Progress: " + progress + "% "),
    contacts => console.log("Contacts: ", contacts)
);
```