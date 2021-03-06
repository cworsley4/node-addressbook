//
// Wire
// Copyright (C) 2016 Wire Swiss GmbH
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.



#include "Person.h"
#include "Group.h"

#ifdef __APPLE__
#include "AddressBook/ABAddressBookC.h"
#endif

#include "AddressBook.h"

AddressBook::AddressBook()
{
}

Person* AddressBook::getMe() const
{
#ifdef __APPLE__
	ABAddressBookRef ab = ABGetSharedAddressBook();
	ABPersonRef me = ABGetMe(ab);
	Person *p = new Person(me);
#else
	Person *p = new Person();
#endif
	return p;
}

unsigned long AddressBook::groupCount() const
{
	#ifdef __APPLE__
		CFIndex count = 0;
		ABAddressBookRef ab = ABGetSharedAddressBook();
		CFArrayRef groups = ABCopyArrayOfAllGroups(ab);
		if (groups) {
			count = CFArrayGetCount(groups);
			CFRelease(groups);
		}
		return count;
	#else
		return 0;
	#endif
}

Group* AddressBook::getGroup(unsigned long pos) const
{
#ifdef __APPLE__
	Group *g = NULL;
	ABAddressBookRef ab = ABGetSharedAddressBook();
	CFArrayRef groups = ABCopyArrayOfAllGroups(ab);
	if (groups) {
		CFIndex count = CFArrayGetCount(groups);
		if ((CFIndex)pos < count) {
			ABGroupRef ge = (ABGroupRef)CFArrayGetValueAtIndex(groups, pos);
			if (ge) {
				g = new Group(ge);
			}
		}
		CFRelease(groups);
	}
#else
	Group *g = new Group();
#endif
	return g;
}

unsigned long AddressBook::contactCount() const
{
#ifdef __APPLE__
	CFIndex count = 0;
	ABAddressBookRef ab = ABGetSharedAddressBook();
	CFArrayRef peeps = ABCopyArrayOfAllPeople(ab);
	if (peeps) {
		count = CFArrayGetCount(peeps);
		CFRelease(peeps);
	}
	return count;
#else
	return 0;
#endif
}

Person* AddressBook::getContact(unsigned long pos) const
{
#ifdef __APPLE__
	Person *p = NULL;
	ABAddressBookRef ab = ABGetSharedAddressBook();
	CFArrayRef peeps = ABCopyArrayOfAllPeople(ab);
	if (peeps) {
		CFIndex count = CFArrayGetCount(peeps);
		if ((CFIndex)pos < count) {
			ABPersonRef pe = (ABPersonRef)CFArrayGetValueAtIndex(peeps, pos);
			if (pe) {
				p = new Person(pe);
			}
		}
		CFRelease(peeps);
	}
#else
	Person *p = new Person();
#endif
	return p;
}

std::vector<Person *> AddressBook::getAllContacts() const {
#ifdef __APPLE__
    std::vector<Person*> persons;
    Person *p = NULL;
    ABAddressBookRef ab = ABGetSharedAddressBook();
    CFArrayRef peeps = ABCopyArrayOfAllPeople(ab);
    if (peeps) {
        CFIndex count = CFArrayGetCount(peeps);

        for (int i = 0; i < (int)count; i++) {
            ABPersonRef pe = (ABPersonRef)CFArrayGetValueAtIndex(peeps, i);
            if (pe) {
                p = new Person(pe);
                persons.push_back(p);
            }
        }
    }

    CFRelease(peeps);
#else
    std::vector < Person * > persons;
#endif
    return persons;
}

