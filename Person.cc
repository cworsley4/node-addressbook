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
#include "AddressBook.h"

#ifdef __APPLE__
std::string Person::CFString2String(CFStringRef str)
{
        std::string rv;
        CFIndex length = CFStringGetLength(str);
        CFIndex maxSize = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8) + 1;
        char *buffer = (char *)malloc(maxSize);
        if (CFStringGetCString(str, buffer, maxSize, kCFStringEncodingUTF8)) {
                rv = buffer;
		free(buffer);
        }   

        return rv; 
}

std::string Person::getStringProperty(ABPersonRef person, CFStringRef propertyName)
{
	CFStringRef propertyVal = (CFStringRef)ABRecordCopyValue(person, propertyName);
	std::string rv;

	if (propertyVal && CFGetTypeID(propertyVal) == CFStringGetTypeID()) {
		rv = CFString2String(propertyVal);
		CFRelease(propertyVal);
	}   

	return rv;
}

void Person::fillPropertyVector(ABPersonRef person, CFStringRef propertyName, stringvector& vec)
{
        ABMultiValueRef propertyArray = (ABMultiValueRef)ABRecordCopyValue(person, propertyName);

        if (propertyArray) {
                CFIndex count = ABMultiValueCount(propertyArray);
                for(CFIndex p = 0; p < count; p++) {
                        CFStringRef propertyVal = (CFStringRef)ABMultiValueCopyValueAtIndex(propertyArray, p);
			vec.push_back(CFString2String(propertyVal));
                        CFRelease(propertyVal);
                }   
        }
}

void Person::fillGroupsVector(ABPersonRef person, CFStringRef propertyName, stringvector& vec)
{
        CFArrayRef arrayRef = (CFArrayRef)ABPersonCopyParentGroups(person);

        if (arrayRef) {
                CFIndex count = CFArrayGetCount(arrayRef);
                for(CFIndex g = 0; g < count; g++) {
                        ABGroupRef group = (ABGroupRef)CFArrayGetValueAtIndex(arrayRef, g);
                        CFStringRef propertyVal = (CFStringRef)ABRecordCopyValue(group, propertyName);
                        vec.push_back(CFString2String(propertyVal));
                        CFRelease(propertyVal);
                } 
        }
}

void Person::fillPhoneVector(ABPersonRef person, phonevector& vec)
{
        ABMultiValueRef propertyArray = (ABMultiValueRef)ABRecordCopyValue(person, kABPhoneProperty);

        if (propertyArray) {
                CFIndex count = ABMultiValueCount(propertyArray);
                for(CFIndex p = 0; p < count; p++) {
                        phonemap pm;
                        CFStringRef propertyPhoneNumberLabel = (CFStringRef)ABMultiValueCopyLabelAtIndex(propertyArray, p);
                        CFStringRef propertyPhoneNumberLabelLocalized = (CFStringRef)ABCopyLocalizedPropertyOrLabel(propertyPhoneNumberLabel);
                        CFStringRef propertyId = (CFStringRef)ABMultiValueCopyIdentifierAtIndex(propertyArray, p);
                        CFStringRef propertyVal = (CFStringRef)ABMultiValueCopyValueAtIndex(propertyArray, p);
			pm.insert(std::pair<std::string, std::string>("number", CFString2String(propertyVal)));
                        pm.insert(std::pair<std::string, std::string>("uuid", CFString2String(propertyId)));
                        pm.insert(std::pair<std::string, std::string>("label", CFString2String(propertyPhoneNumberLabelLocalized)));
                        vec.push_back(pm);
                        CFRelease(propertyPhoneNumberLabel);
                        CFRelease(propertyPhoneNumberLabelLocalized);
                        CFRelease(propertyId);
                        CFRelease(propertyVal);
                }   
        }
}
#endif

Person::Person()
{
}

#ifdef __APPLE__
Person::Person(ABPersonRef p)
{
        
	m_firstName = getStringProperty(p, kABFirstNameProperty);
	m_lastName = getStringProperty(p, kABLastNameProperty);
        m_group = getStringProperty(p, kABGroupNameProperty);
        m_uuid = getStringProperty(p, kABUIDProperty);

	fillGroupsVector(p, kABUIDProperty, m_groups);
	fillPropertyVector(p, kABEmailProperty, m_emails);
        fillPhoneVector(p, m_numbers);
}
#endif

const phonevector& Person::numbers() const
{
	return m_numbers;
}

const stringvector& Person::emails() const
{
	return m_emails;
}

const stringvector& Person::groups() const
{
        return m_groups;
}