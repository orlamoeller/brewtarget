/*╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌
 * utils/PropertyPath.cpp is part of Brewtarget, and is copyright the following authors 2023-2024:
 *   • Matt Young <mfsy@yahoo.com>
 *
 * Brewtarget is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Brewtarget is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 ╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌*/
#include "utils/PropertyPath.h"

#include "model/NamedEntity.h"

PropertyPath::PropertyPath(BtStringConst const & singleProperty) :
   m_properties{1, &singleProperty}, m_path{*singleProperty} {
   return;
}

PropertyPath::PropertyPath(std::initializer_list<std::reference_wrapper<BtStringConst const>> listOfProperties) :
   m_properties{}, m_path{} {
   bool first = true;
   for (auto const & ii : listOfProperties) {
      m_properties.append(&ii.get());
      if (!first) {
         m_path.append("/");
      }
      first = false;
      m_path.append(*ii.get());
   }
   return;
}

PropertyPath::PropertyPath(PropertyPath const & other) :
   m_properties{other.m_properties},
   m_path      {other.m_path      } {
   return;
}

PropertyPath & PropertyPath::operator=(PropertyPath const & other) {
   if (this != &other) {
      m_properties = other.m_properties;
      m_path       = other.m_path      ;
   }
   return *this;
}


PropertyPath::~PropertyPath() = default;

QString PropertyPath::asXPath() const {
   return this->m_path;
}

QVector<BtStringConst const *> const & PropertyPath::properties() const {
   return this->m_properties;
}

bool PropertyPath::isNull() const {
   if (this->m_properties.size() == 1) {
      return BtString::NULL_STR == *this->m_properties[0];
   }
   return false;
}

TypeInfo const & PropertyPath::getTypeInfo(TypeLookup const & baseTypeLookup) const {
   TypeInfo const * returnValue = nullptr;
   // Can be useful to uncomment the next line for debugging
//   qDebug() << Q_FUNC_INFO << "Applying PropertyPath" << *this << "to" << baseTypeLookup;

   TypeLookup const * typeLookup = &baseTypeLookup;
   for (auto const property : this->m_properties) {
      TypeInfo const & typeInfo = typeLookup->getType(*property);
      if (property == this->m_properties.last()) {
         returnValue = &typeInfo;
         break;
      }
      typeLookup = typeInfo.typeLookup;
      // It's a coding error if there is no TypeLookup
      if (!typeLookup) {
         qCritical() <<
            Q_FUNC_INFO << "Applying PropertyPath" << *this << "to" << baseTypeLookup << ": no TypeLookup for" <<
            *property << ". (getType returned " << typeInfo << ")";
         Q_ASSERT(false);
      }
   }

   return *returnValue;

}

[[nodiscard]] bool PropertyPath::setValue(NamedEntity & obj, QVariant const & val) const {
   NamedEntity * ne = &obj;
   for (auto const property : this->m_properties) {
      if (property == this->m_properties.last()) {

         // It's a coding error if we're trying to set a non-existent property on the NamedEntity subclass for this
         // record.
         QMetaObject const * neMetaObject = ne->metaObject();
         int propertyIndex = neMetaObject->indexOfProperty(**property);
         Q_ASSERT(propertyIndex >= 0);
         QMetaProperty neMetaProperty = neMetaObject->property(propertyIndex);

      // Normally keep this log statement commented out otherwise it generates too many lines in the log file
//         qDebug() <<
//            Q_FUNC_INFO << "Request to set" << this->m_path << "on" << obj.metaObject()->className() << "(=" <<
//            *property << "on" << ne->metaObject()->className() << "); property type =" << neMetaProperty.typeName() <<
//            "; writable =" << neMetaProperty.isWritable();

         if (neMetaProperty.isWritable()) {
            bool succeeded = ne->setProperty(**property, val);
            if (!succeeded) {
               // Caller needs to decide what to do, but we assume it's a coding error that the property could not be
               // set.
               qCritical() <<
                  Q_FUNC_INFO << "Error trying to set" << this->m_path << "on" << obj.metaObject()->className() <<
                  "(=" << *property << "on" << ne->metaObject()->className() << ") to" << val << "; property type =" <<
                  neMetaProperty.typeName() << "; writable =" << neMetaProperty.isWritable();
            }
            return succeeded;
         }
         break;
      }

      ne = ne->ensureExists(*property);
   }

   return false;
}

QVariant PropertyPath::getValue(NamedEntity const & obj) const {
   QVariant retVal{};
   NamedEntity const * ne = &obj;
   for (auto const property : this->m_properties) {
      // Normally keep the next line commented out otherwise it generates too many lines in the log file
//      qDebug() << Q_FUNC_INFO << "Looking at" << *property;

      if (property == this->m_properties.last()) {
         //
         // We've chained through the properties and found the end one that we want the actual value of
         //
         QMetaObject const * neMetaObject = ne->metaObject();
         // Uncomment the next line if the assert below is firing
//         qDebug() <<
//            Q_FUNC_INFO << "Request to get" << this->m_path << "on" << obj.metaObject()->className() << "(=" <<
//            *property << "on" << ne->metaObject()->className() << ")";

         // It's a coding error if we're trying to get a non-existent property on the NamedEntity subclass for this
         // record.
         int propertyIndex = neMetaObject->indexOfProperty(**property);
         Q_ASSERT(propertyIndex >= 0);
         QMetaProperty neMetaProperty = neMetaObject->property(propertyIndex);

         // Normally keep this log statement commented out otherwise it generates too many lines in the log file
//         qDebug() <<
//            Q_FUNC_INFO << "Request to get" << this->m_path << "on" << obj.metaObject()->className() << "(=" <<
//            *property << "on" << ne->metaObject()->className() << "); property type =" << neMetaProperty.typeName() <<
//            "; readable =" << neMetaProperty.isReadable();

         if (neMetaProperty.isReadable()) {
            retVal = ne->property(**property);
            if (!retVal.isValid()) {
               auto mo = ne->metaObject();
               qWarning() <<
                  Q_FUNC_INFO << "Property" << *property << "on" << mo->className() << "#" << ne->key() <<
                  "not readable.  Property Index =" << mo->indexOfProperty(**property);
            }
         }
         break;
      }

      //
      // Note that we can't fall through from the above "if" here.
      //
      // This is a property prior to the last one in the chain.  So it's some sort of pointer to an object which is a
      // subclass of NamedEntity.
      //
      // Although we hope one day to be able to simplify things (when we have thought more profoundly about object
      // lifetimes and ownership), for the moment, the pointer could be either a raw pointer or a shared one.  For a raw
      // pointer, life is relatively easy as we can safely downcast it to a `NamedEntity *` and then ask it to give us
      // the Qt Property we are after (next time around the loop).  For a shared pointer, things are a bit more
      // complicated and we need some help from TypeInfo to obtain a `NamedEntity *`.  Either way, we need to get the
      // TypeInfo object first to find out what sort of pointer we're dealing with.
      //
      QVariant containedNe = ne->property(**property);
      TypeInfo const & typeInfo = ne->getTypeLookup().getType(*property);
      switch (typeInfo.pointerType) {
         case TypeInfo::PointerType::RawPointer:
            // In this case, what we are expecting inside the containedNe QVariant is `NamedEntity *`.  It's OK for
            // us to assign this pointer to a variable of type `NamedEntity const *`, but we shouldn't expect
            // `containedNe.canConvert<NamedEntity const *>()` to return true (because it won't).
            if (!containedNe.isValid() || !containedNe.canConvert<NamedEntity *>()) {
               break;
            }
            ne = containedNe.value<NamedEntity *>();
            break;

         case TypeInfo::PointerType::SharedPointer:
            //
            // Inside the QVariant will be std::shared_ptr<NE> where NE is some subclass of NamedEntity.  We want to
            // convert it to something known, which we can do by giving the QVariant to the relevant
            // NamedEntityCasters::downcastPointer function.
            //
            // First we assert that it's a coding error if we don't have caster functions in the type info for a
            // shared pointer type.
            //
            Q_ASSERT(typeInfo.namedEntityCasters);
            ne = typeInfo.namedEntityCasters->m_pointerDowncaster(containedNe).get();
            break;

         case TypeInfo::PointerType::NotPointer:
            // This is a coding error, as we shouldn't have something that's not a pointer at all as an "internal" node
            // in a property path.
            qCritical() << Q_FUNC_INFO << *property << "on" << ne << "is not pointer!";
            Q_ASSERT(false);
            // This forces an exit below
            ne = nullptr;
            break;

         // No default case as we want compiler to warn us if we missed an enumeration
      }

      if (!ne) {
         qDebug() << Q_FUNC_INFO << "Property" << *property << "returned nullptr";
         break;
      }
   }

   // Normally keep the next line commented out otherwise it generates too many lines in the log file
//   qDebug() << Q_FUNC_INFO << "Returning" << retVal;

   return retVal;
}
