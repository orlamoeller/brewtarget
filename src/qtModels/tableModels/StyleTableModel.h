/*╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌
 * qtModels/tableModels/StyleTableModel.h is part of Brewtarget, and is copyright the following authors 2023:
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
#ifndef TABLEMODELS_STYLETABLEMODEL_H
#define TABLEMODELS_STYLETABLEMODEL_H
#pragma once

#include <QItemDelegate>
#include <QMetaProperty>
#include <QModelIndex>
#include <QVariant>
#include <QWidget>

#include "model/Style.h"
#include "qtModels/tableModels/ItemDelegate.h"
#include "qtModels/tableModels/TableModelBase.h"

class BtStringConst;
class Recipe;

// You have to get the order of everything right with traits classes, but the end result is that we can refer to
// StyleTableModel::ColumnIndex::Type etc.
class StyleTableModel;
template <> struct TableModelTraits<StyleTableModel> {
   enum class ColumnIndex {
      Name          ,
      Type          ,
      Category      ,
      CategoryNumber,
      StyleLetter   ,
      StyleGuide    ,
   };
};

/*!
 * \class StyleTableModel
 *
 * \brief Table model for a list of styles.
 */
class StyleTableModel : public BtTableModel, public TableModelBase<StyleTableModel, Style> {
   Q_OBJECT

   TABLE_MODEL_COMMON_DECL(Style)
};

//============================================== CLASS StyleItemDelegate ===============================================
/*!
 * \class StyleItemDelegate
 *
 * \brief Item delegate for style tables.
 * \sa StyleTableModel
 */
class StyleItemDelegate : public QItemDelegate,
                          public ItemDelegate<StyleItemDelegate, StyleTableModel> {
   Q_OBJECT

   ITEM_DELEGATE_COMMON_DECL(Style)
};

#endif
