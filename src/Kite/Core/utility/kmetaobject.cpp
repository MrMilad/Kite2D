/*
Kite2D Game Framework.
Copyright (C) 2010-2015  Milad Rasaneh <milad_rasaneh2000@yahoo.com>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
USA
*/
#include "Kite/Core/utility/kmetaobject.h"

namespace Kite {
	KMetaObject::KMetaObject(const std::string &Name, U32 Size, U32 Flag) :
		_kname(Name), _ksize(Size), _kflag(Flag),
		_kmembers(NULL), _klastMember(NULL),
		_kproperties(NULL), _klastProperties(NULL) 
	{}

	KMetaObject::~KMetaObject() {
		// free members
		if (hasMembers()) {
			KMetaMember *mem = _kmembers;
			while (mem != NULL) {
				KMetaMember *temp = mem->next;
				delete mem;
				mem = temp;
			}
		}

		// free properties
		if (hasProperties()) {
			KMetaProperty *prp = _kproperties;
			while (prp != NULL) {
				KMetaProperty *temp = prp->next;
				delete prp;
				prp = temp;
			}
		}

		// free base
		if (hasBase()) {
			KMetaBase *base = _kbases;
			while (base != NULL) {
				KMetaBase *temp = base->next;
				delete base;
				base = temp;
			}
		}
	}

	void KMetaObject::addMember(const KMetaMember *Member) {
		if (!_kmembers)
			_kmembers = const_cast<KMetaMember *>(Member);
		else
			_klastMember->next = const_cast<KMetaMember *>(Member);

		_klastMember = const_cast<KMetaMember *>(Member);
	}

	void KMetaObject::addProperty(const KMetaProperty *Property) {
		if (!_kproperties)
			_kproperties = const_cast<KMetaProperty *>(Property);
		else
			_klastProperties->next = const_cast<KMetaProperty *>(Property);

		_klastProperties = const_cast<KMetaProperty *>(Property);
	}

	void KMetaObject::addBase(const KMetaBase *Base) {
		if (!_kbases)
			_kbases = const_cast<KMetaBase *>(Base);
		else
			_klastBase->next = const_cast<KMetaBase *>(Base);

		_klastBase = const_cast<KMetaBase *>(Base);
	}
}