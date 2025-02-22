/*╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌╌
 * model/Recipe.h is part of Brewtarget, and is copyright the following authors 2009-2024:
 *   • Brian Rower <brian.rower@gmail.com>
 *   • Greg Meess <Daedalus12@gmail.com>
 *   • Jeff Bailey <skydvr38@verizon.net>
 *   • Kregg Kemper <gigatropolis@yahoo.com>
 *   • Mattias Måhl <mattias@kejsarsten.com>
 *   • Matt Young <mfsy@yahoo.com>
 *   • Mik Firestone <mikfire@gmail.com>
 *   • Philip Greggory Lee <rocketman768@gmail.com>
 *   • Théophane Martin <theophane.m@gmail.com>
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
#ifndef MODEL_RECIPE_H
#define MODEL_RECIPE_H
#pragma once

#include <memory> // For PImpl

#include <QColor>
#include <QDate>
#include <QList>
#include <QMutex>
#include <QSqlRecord>
#include <QString>
#include <QVariant>
#include <QVector>

#include "database/ObjectStoreWrapper.h"
#include "model/FolderBase.h"
#include "model/NamedEntity.h"
#include "model/OwnedSet.h"

//======================================================================================================================
//========================================== Start of property name constants ==========================================
// See comment in model/NamedEntity.h
#define AddPropertyName(property) namespace PropertyNames::Recipe { inline BtStringConst const property{#property}; }
AddPropertyName(ABV_pct                )
AddPropertyName(age_days               )
AddPropertyName(ageTemp_c              )
AddPropertyName(ancestorId             )
AddPropertyName(apparentAttenuation_pct)
AddPropertyName(asstBrewer             )
AddPropertyName(batchSize_l            )
AddPropertyName(beerAcidity_pH         )
AddPropertyName(boil                   )
AddPropertyName(boilGrav               )
AddPropertyName(boilId                 )
AddPropertyName(boilVolume_l           )
AddPropertyName(brewer                 )
AddPropertyName(brewNotes              )
AddPropertyName(calcsEnabled           )
AddPropertyName(caloriesPer33cl        )
AddPropertyName(caloriesPerLiter       )
AddPropertyName(caloriesPerUs12oz      )
AddPropertyName(caloriesPerUsPint      )
AddPropertyName(carbonationTemp_c      )
AddPropertyName(carbonation_vols       )
AddPropertyName(color_srm              )
AddPropertyName(date                   )
AddPropertyName(efficiency_pct         )
AddPropertyName(equipment              )
AddPropertyName(equipmentId            )
///AddPropertyName(fermentableAdditionIds )
AddPropertyName(fermentableAdditions   )
AddPropertyName(fermentation           )
AddPropertyName(fermentationId         )
AddPropertyName(fg                     )
AddPropertyName(finalVolume_l          )
AddPropertyName(forcedCarbonation      )
AddPropertyName(grainsInMash_kg        )
AddPropertyName(grains_kg              )
///AddPropertyName(hopAdditionIds         )
AddPropertyName(hopAdditions           )
AddPropertyName(IBU                    )
AddPropertyName(IBUs                   )
///AddPropertyName(instructionIds         )
AddPropertyName(instructions           )
AddPropertyName(kegPrimingFactor       )
AddPropertyName(locked                 )
AddPropertyName(mash                   )
AddPropertyName(mashId                 )
///AddPropertyName(miscAdditionIds        )
AddPropertyName(miscAdditions          )
AddPropertyName(notes                  )
AddPropertyName(og                     )
AddPropertyName(points                 )
AddPropertyName(postBoilVolume_l       )
AddPropertyName(primingSugarEquiv      )
AddPropertyName(primingSugarName       )
///AddPropertyName(saltAdjustmentIds      )
AddPropertyName(saltAdjustments        )
AddPropertyName(SRMColor               )
AddPropertyName(style                  )
AddPropertyName(styleId                )
AddPropertyName(tasteNotes             )
AddPropertyName(tasteRating            )
AddPropertyName(type                   )
///AddPropertyName(waterUseIds            )
AddPropertyName(waterUses              )
AddPropertyName(wortFromMash_l         )
///AddPropertyName(yeastAdditionIds       )
AddPropertyName(yeastAdditions         )
#undef AddPropertyName
//=========================================== End of property name constants ===========================================
//======================================================================================================================

// Forward declarations
class Boil;
class BoilStep;
class BrewNote;
class Equipment;
class Fermentable;
class Fermentation;
class FermentationStep;
class Instruction;
class Mash;
class MashStep;
class RecipeAdditionFermentable;
class RecipeAdditionHop;
class RecipeAdditionMisc;
class RecipeAdjustmentSalt;
class RecipeAdditionYeast;
class RecipeUseOfWater;
class Style;
class Water;
class Yeast;


/*!
 * \class Recipe
 *
 * \brief Model class for recipe records in the database.
 */
class Recipe : public NamedEntity,
               public FolderBase<Recipe> {
   Q_OBJECT
   FOLDER_BASE_DECL(Recipe)

   // See model/FolderBase.h for info, getters and setters for these properties
   Q_PROPERTY(QString folder        READ folder        WRITE setFolder     )

   /**
    * \brief \c MainWindow is a friend so it can access \c Recipe::recalcAll() and \c Recipe::recalcIfNeeded()
    *        \c BrewDayScrollWidget is a friend so it can access \c Recipe::m_instructions
    *
    *        In the long run, we should fix this, so that \c MainWindow doesn't need to call private member functions on
    *        \c Recipe.
    */
   friend class MainWindow;
   friend class BrewDayScrollWidget;

public:
   /**
    * \brief See comment in model/NamedEntity.h
    */
   static QString localisedName();

   /**
    * \brief Mapping of names to types for the Qt properties of this class.  See \c NamedEntity::typeLookup for more
    *        info.
    */
   static TypeLookup const typeLookup;
   TYPE_LOOKUP_GETTER

   Recipe(QString name);
   Recipe(NamedParameterBundle const & namedParameterBundle);
   Recipe(Recipe const & other);

   virtual ~Recipe();

    //! \brief the user can select what delete means
   enum delOptions {
      ANCESTOR,   // delete the recipe and all its ancestors
      DESCENDANT  // delete only the recipe (orphan and delete)
   };

   //! \brief The type of recipe
   enum class Type {
      Extract    ,
      PartialMash,
      AllGrain   ,
      // ⮜⮜⮜ All below added for BeerJSON support ⮞⮞⮞
      Cider      ,
      Kombucha   ,
      Soda       ,
      Other      ,
      Mead       ,
      Wine       ,
   };
   // This allows us to store the above enum class in a QVariant
   Q_ENUM(Type)

   /*!
    * \brief Mapping between \c Recipe::Type and string values suitable for serialisation in DB, BeerJSON, etc (but
    *        \b not BeerXML)
    *
    *        This can also be used to obtain the number of values of \c Type, albeit at run-time rather than
    *        compile-time.  (One day, C++ will have reflection and we won't need to do things this way.)
    */
   static EnumStringMapping const typeStringMapping;

   /*!
    * \brief Localised names of \c Recipe::Type values suitable for displaying to the end user
    */
   static EnumStringMapping const typeDisplayNames;


   //=============================================== REGULAR PROPERTIES ================================================
   //
   // Note that boilSize_l and boilTime_min, which were previously properties of Recipe are now moved to Boil.  Given
   // Recipe r:
   //   - to SET a boil size of `double boilSizeLiters`, call `r.nonOptBoil()->setPreBoilSize_l(boilSizeLiters)`;
   //   - to SET a boil time of `double boilTimeMinutes`, call `r.nonOptBoil()->setBoilTime_mins(boilTimeMinutes)`.
   //
   //! \brief The \b Type
   Q_PROPERTY(Type    type               READ type               WRITE setType             )
   //! \brief The brewer.  This becomes "author" in BeerJSON
   Q_PROPERTY(QString brewer             READ brewer             WRITE setBrewer           )
   /**
    * \brief The batch size is the target size of the finished batch (in liters) aka the volume into the fermenter.
    */
   Q_PROPERTY(double  batchSize_l        READ batchSize_l        WRITE setBatchSize_l      )
   //! \brief The overall efficiency in percent.
   Q_PROPERTY(double  efficiency_pct     READ efficiency_pct     WRITE setEfficiency_pct   )
   //! \brief The assistant brewer.  This becomes "coauthor" in BeerJSON
   Q_PROPERTY(QString asstBrewer         READ asstBrewer         WRITE setAsstBrewer       )
   //! \brief The notes.
   Q_PROPERTY(QString notes              READ notes              WRITE setNotes            )
   //! \brief The tasting notes.
   Q_PROPERTY(QString tasteNotes         READ tasteNotes         WRITE setTasteNotes       )
   /**
    * \brief Decimal number between zero and 50.0 denoting the taste rating – corresponds to the 50 point BJCP rating
    *         system.
    *         .:TBD:. This is stored as a double but the UI constrains it to an unsigned int.
    */
   Q_PROPERTY(double  tasteRating        READ tasteRating        WRITE setTasteRating      )
   /**
    * \brief The number of days to age the beer after bottling.  This is an optional field in BeerXML, but is not
    *        directly part of BeerJSON. .:TBD:. We should probably map this and ageTemp_c to the BeerJSON equivalent --
    *        just as soon as we work out what that is!
    */
   Q_PROPERTY(std::optional<double> age_days           READ age_days           WRITE setAge_days         )
   //! \brief The temp in C as beer is ageing after bottling.
   Q_PROPERTY(std::optional<double> ageTemp_c          READ ageTemp_c          WRITE setAgeTemp_c        )
   /**
    * \brief In BeerXML, a recipe has a date which is supposed to be when it was brewed.  This is slightly meaningless
    *        unless you take it to mean "first brewed".  We then take that to be the "created" date in BeerJSON and our
    *        UI.
    *        NB: In both BeerXML and BeerJSON, this is an optional field
    */
   Q_PROPERTY(std::optional<QDate>   date               READ date               WRITE setDate             )
   /**
    * \brief The carbonation in volumes of CO2 at standard temperature and pressure (STP).
    *        NB: In both BeerXML and BeerJSON, this is an optional field
    */
   Q_PROPERTY(std::optional<double>  carbonation_vols   READ carbonation_vols   WRITE setCarbonation_vols )
   //! \brief Whether the beer is force carbonated.
   Q_PROPERTY(bool    forcedCarbonation  READ forcedCarbonation  WRITE setForcedCarbonation)
   /**
    * \brief The name of the priming sugar.
    *
    *        TBD: This is not currently exposed in the UI.
    *
    *        The field is optional in BeerXML and not supported in BeerJSON (where instead you would have an extra entry
    *        in fermentable_additions with timing.use == add_to_package).
    */
   Q_PROPERTY(QString primingSugarName   READ primingSugarName   WRITE setPrimingSugarName )
   //! \brief The temperature in C while carbonating.
   Q_PROPERTY(double  carbonationTemp_c  READ carbonationTemp_c  WRITE setCarbonationTemp_c)
   //! \brief The factor required to convert this priming agent to an equivalent amount of glucose monohyrate.
   Q_PROPERTY(double  primingSugarEquiv  READ primingSugarEquiv  WRITE setPrimingSugarEquiv)
   //! \brief The factor required to convert the amount of sugar required for bottles to keg (usually about 0.5).
   Q_PROPERTY(double  kegPrimingFactor   READ kegPrimingFactor   WRITE setKegPrimingFactor )
   //! \brief Whether the recipe is locked against changes
   Q_PROPERTY(bool    locked             READ locked             WRITE setLocked           )

   /**
    * \brief Whether calculations are enabled.  By default, all the automatic calculations are enabled.  However, it is
    *        helpful to be able to disable them when we are reading a recipe in from serialisation, otherwise, we can
    *        end up trying to calculate things before we've finished reading in data (eg for fermentable additions).
    *
    *        TBD: Should perhaps move this up to \c NamedEntity.
    */
   Q_PROPERTY(bool calcsEnabled   READ calcsEnabled   WRITE setCalcsEnabled   STORED false)

   // ⮜⮜⮜ All below added for BeerJSON support ⮞⮞⮞
   //! \brief The final beer pH at the end of fermentation.
   Q_PROPERTY(std::optional<double> beerAcidity_pH               READ beerAcidity_pH                WRITE setBeerAcidity_pH)
   //! \brief The total apparent attenuation of the finished beer after fermentation.
   Q_PROPERTY(std::optional<double> apparentAttenuation_pct      READ apparentAttenuation_pct       WRITE setApparentAttenuation_pct)


   //=========================================== CALCULATED STORED PROPERTIES ==========================================
   // These are optional in both BeerXML and BeerJSON, but they don't need to be optional here.  When reading in from
   // BeerXML or BeerJSON, we ignore the values if they are present, as we'll recalculate them ourselves.
   //
   // Same goes for ABV_pct, IBU, color_srm below (which are optional BeerJSON fields but not part of BeerXML)
   //
   //! \brief The calculated OG
   Q_PROPERTY(double  og                 READ og                 WRITE setOg)
   //! \brief The calculated FG
   Q_PROPERTY(double  fg                 READ fg                 WRITE setFg)

   //========================================= CALCULATED UNSTORED PROPERTIES ==========================================
   // These need to listen for changes to the uncalculated properties they depend on, and re-emit changed() when
   // appropriate.
   //! \brief The calculated points (1000*(\c og()-1.0)).
   Q_PROPERTY(double  points              READ points              STORED false)
   //! \brief The calculated ABV in percent.
   Q_PROPERTY(double  ABV_pct             READ ABV_pct             STORED false)
   //! \brief The calculated color in SRM.
   Q_PROPERTY(double  color_srm           READ color_srm           STORED false)
   //! \brief The calculated boil gravity. .:TBD:. This should perhaps be renamed boilSg for consistency
   Q_PROPERTY(double  boilGrav            READ boilGrav            STORED false)
   //! \brief The calculated IBUs.
   Q_PROPERTY(double  IBU                 READ IBU                 STORED false)
   //! \brief IBU contributions from each hop.
   Q_PROPERTY(QList<double> IBUs          READ IBUs                STORED false)
   //! \brief The calculated wort coming from the mash in liters.
   Q_PROPERTY(double  wortFromMash_l      READ wortFromMash_l      STORED false)
   //! \brief The calculated preboil volume in liters.
   Q_PROPERTY(double  boilVolume_l        READ boilVolume_l        STORED false)
   //! \brief The calculated postboil volume in liters.
   Q_PROPERTY(double  postBoilVolume_l    READ postBoilVolume_l    STORED false)
   //! \brief The calculated final volume into the primary in liters.
   Q_PROPERTY(double  finalVolume_l       READ finalVolume_l       STORED false)
   //! \brief The calculated Calories per liter. (kcal).
   Q_PROPERTY(double  caloriesPerLiter    READ caloriesPerLiter    STORED false)
   //! \brief The calculated Calories per 33cl. (kcal).
   Q_PROPERTY(double  caloriesPer33cl     READ caloriesPer33cl     STORED false)
   //! \brief The calculated Calories per (US) 12 oz. (kcal).
   Q_PROPERTY(double  caloriesPerUs12oz   READ caloriesPerUs12oz   STORED false)
   //! \brief The calculated Calories per US pint (16 US fl oz). (kcal).
   Q_PROPERTY(double  caloriesPerUsPint   READ caloriesPerUsPint   STORED false)
   //! \brief The amount of grains in the mash in kg.
   Q_PROPERTY(double  grainsInMash_kg     READ grainsInMash_kg     STORED false)
   //! \brief The total amount of grains in the recipe in kg.
   Q_PROPERTY(double  grains_kg           READ grains_kg           STORED false)
   //! \brief The beer color as a displayable QColor.
   Q_PROPERTY(QColor  SRMColor            READ SRMColor            STORED false)

   //============================================== RELATIONAL PROPERTIES ==============================================
   // NB: the setBlahId() calls are needed by ObjectStore and are not intended for more general use.
   // NB: Each of Mash, Boil, Fermentation is technically optional on a Recipe, so it is valid for getters to return
   //     null pointer (aka "empty shared_ptr").
   Q_PROPERTY(std::shared_ptr<Mash        > mash            READ mash               WRITE setMash           STORED false)
   Q_PROPERTY(int                           mashId          READ getMashId          WRITE setMashId)
   Q_PROPERTY(std::shared_ptr<Boil        > boil            READ boil               WRITE setBoil           STORED false)
   Q_PROPERTY(int                           boilId          READ getBoilId          WRITE setBoilId)
   Q_PROPERTY(std::shared_ptr<Fermentation> fermentation    READ fermentation       WRITE setFermentation   STORED false)
   Q_PROPERTY(int                           fermentationId  READ getFermentationId  WRITE setFermentationId)
   //! \brief \c Equipment an optional \c Recipe field in BeerXML but is not part of \c Recipe at all in BeerJSON
   Q_PROPERTY(std::shared_ptr<Equipment   > equipment       READ equipment          WRITE setEquipment      STORED false)
   Q_PROPERTY(int            equipmentId     READ getEquipmentId     WRITE setEquipmentId)
   Q_PROPERTY(std::shared_ptr<Style       > style           READ style              WRITE setStyle          STORED false)
   Q_PROPERTY(int            styleId         READ getStyleId         WRITE setStyleId)

   // These QList properties should only emit changed() when their size changes, or when
   // one of their elements is replaced by another with a different key.
   //! \brief The fermentable additions.
   Q_PROPERTY(QList<std::shared_ptr<RecipeAdditionFermentable>> fermentableAdditions   READ fermentableAdditions WRITE setFermentableAdditions   STORED false)
   //! \brief The hop additions.
   Q_PROPERTY(QList<std::shared_ptr<RecipeAdditionHop>> hopAdditions   READ hopAdditions WRITE setHopAdditions   STORED false)
   //! \brief The misc additions.
   Q_PROPERTY(QList<std::shared_ptr<RecipeAdditionMisc>> miscAdditions   READ miscAdditions WRITE setMiscAdditions   STORED false)
   //! \brief The yeast additions.
   Q_PROPERTY(QList<std::shared_ptr<RecipeAdditionYeast>> yeastAdditions   READ yeastAdditions WRITE setYeastAdditions   STORED false)
   //! \brief The waters.
   Q_PROPERTY(QList<std::shared_ptr<RecipeUseOfWater>> waterUses   READ waterUses   WRITE setWaterUses   STORED false)
   //! \brief The salt adjustments.
   Q_PROPERTY(QList<std::shared_ptr<RecipeAdjustmentSalt>> saltAdjustments    READ saltAdjustments    WRITE setSaltAdjustments   STORED false)
   //! \brief The brew notes.
   Q_PROPERTY(QList<std::shared_ptr<BrewNote>> brewNotes   READ brewNotes /*WRITE*/ /*NOTIFY changed*/ STORED false)
   //! \brief The instructions.
   Q_PROPERTY(QList<std::shared_ptr<Instruction>> instructions   READ instructions /*WRITE*/ /*NOTIFY changed*/ STORED false)

   Q_PROPERTY(int    ancestorId READ getAncestorId WRITE setAncestorId)
   //! \brief The ancestors.
   Q_PROPERTY(QList<Recipe *> ancestors READ ancestors /*WRITE*/ /*NOTIFY changed*/ STORED false)

   /**
    * \brief We need to override \c NamedEntity::setKey to do some extra ancestor stuff
    */
   virtual void setKey(int key) override;

   /**
    * \brief Connect Fermentable, Hop changed signals etc to their parent Recipes.
    *
    *        This is needed because each Recipe needs to know when one of its constituent parts has been modified, eg
    *        if the alpha acid on a hop is modified then that will affect the recipe's IBU.
    *
    *        Needs to be called \b after all the calls to ObjectStoreTyped<FooBar>::getInstance().loadAll()
    */
   static void connectSignalsForAllRecipes();

   /**
    * \brief Use this for adding \c RecipeAdditionHop, etc.
    *
    *        Note that we don't need an equivalent for \c BrewNote or \c Instruction because they are generated by
    *        \c Recipe itself.
    */
   template<class NE> std::shared_ptr<NE> addAddition(std::shared_ptr<NE> addition);

   /**
    * \brief Use this for removing \c RecipeAdditionHop, etc.
    */
   template<class NE> std::shared_ptr<NE> removeAddition(std::shared_ptr<NE> addition);

   /*!
    * \brief Returns whether \c val is used in this recipe
    */
   template<class T> bool uses(T const & val) const;

   /*!
    * \brief Find the first recipe that uses \c var
    *
    *        See below for specialisations (which have to be outside the class definition).
    */
   template<class T> static Recipe * findFirstThatUses(T const & var) {
      return ObjectStoreWrapper::findFirstMatching<Recipe>( [var](Recipe * rec) {return rec->uses(var);} );
   }

   //! \brief Automagically generate a list of instructions.
   void generateInstructions();

   /**
    * \brief Although a \c Recipe typically generates its own Instructions, which the user can then edit, we need to be
    *        able to set them so we can read in \c Instruction records from BeerXML files (where we include them as an
    *        extension to the BeerXML base standard).
    */
   void add(std::shared_ptr<Instruction> instruction);

   /*!
    * Finds the next ingredient to add that has a time
    * less than time. Changes time to be the time of the found
    * ingredient, or if none are found, -1. Returns a string
    * in the form "Add %1 to %2 at %3".
    */
   QString nextAddToBoil(double & time);

   //! \brief convenience method to set ancestors
   void setAncestor(Recipe & ancestor);

   /**
    * \brief Usually called before deleting a Recipe.  Unlinks this Recipe from its its ancestors (aka previous
    *        versions) and set the most recent of these to be editable again.
    * \return The immediately previous version, or \c nullptr if there is none
    */
   Recipe * revertToPreviousVersion();

   //============================================ "GETTER" MEMBER FUNCTIONS ============================================
   Type    type              () const;
   QString brewer            () const;
   double  batchSize_l       () const;
   double  efficiency_pct    () const;
   QString asstBrewer        () const;
   QString notes             () const;
   QString tasteNotes        () const;
   double  tasteRating       () const;
   double  og                () const;
   double  fg                () const;
   int     fermentationStages() const;
   double  primaryAge_days   () const;
   double  secondaryAge_days () const;
   double  secondaryTemp_c   () const;
   double  tertiaryAge_days  () const;
   double  tertiaryTemp_c    () const;
   std::optional<double>  age_days          () const;
   std::optional<double>  ageTemp_c         () const;
   std::optional<QDate>   date              () const;
   std::optional<double>  carbonation_vols  () const;
   bool    forcedCarbonation () const;
   QString primingSugarName  () const;
   double  carbonationTemp_c () const;
   double  primingSugarEquiv () const;
   double  kegPrimingFactor  () const;
   bool    locked            () const;
   bool    calcsEnabled      () const;
   // ⮜⮜⮜ All below added for BeerJSON support ⮞⮞⮞
   std::optional<double> beerAcidity_pH()          const;
   std::optional<double> apparentAttenuation_pct() const;

   // Calculated getters.
   double        points                  () const;
   double        ABV_pct                 () const;
   double        color_srm               () const;
   double        boilGrav                () const;
   double        IBU                     () const;
   QColor        SRMColor                () const;
   double        targetCollectedWortVol_l() const;
   double        targetTotalMashVol_l    () const;
   double        wortFromMash_l          () const;
   double        boilVolume_l            () const;
   double        postBoilVolume_l        () const;
   double        finalVolume_l           () const;
   double        caloriesPer33cl         () const;
   double        caloriesPerLiter        () const;
   double        caloriesPerUs12oz       () const;
   double        caloriesPerUsPint       () const;
   double        grainsInMash_kg         () const;
   double        grains_kg               () const;
   QList<double> IBUs                    () const;

   /**
    * \brief It's useful to have this templated way of getting all the \c NamedEntity objects of a particular type (eg
    *        all \c BrewNote or all \c RecipeAdditionHop objects) that are owned by a given \c Recipe.
    */
   template<typename NE> QList<std::shared_ptr<NE>> allOwned() const;

   // Relational getters
   QList<std::shared_ptr<RecipeAdditionFermentable>> fermentableAdditions  () const;
   QList<std::shared_ptr<RecipeAdditionHop>>         hopAdditions          () const;
   QList<std::shared_ptr<RecipeAdditionMisc>>        miscAdditions         () const;
   QList<std::shared_ptr<RecipeAdditionYeast>>       yeastAdditions        () const;
   QList<std::shared_ptr<RecipeAdjustmentSalt>>      saltAdjustments       () const;
   QList<std::shared_ptr<RecipeUseOfWater>>          waterUses             () const;
   QList<std::shared_ptr<BrewNote>>                  brewNotes             () const;
   QList<std::shared_ptr<Instruction>>               instructions          () const;
   QList<Recipe *>                                   ancestors             () const;
   std::shared_ptr<Equipment>                        equipment             () const;
   int                                               getEquipmentId        () const;
   std::shared_ptr<Style>                            style                 () const;
   int                                               getStyleId            () const;

   std::shared_ptr<Mash>                        mash              () const;
   //! \brief This will create a \c Mash object if it doesn't exist
   std::shared_ptr<Mash>                        nonOptMash        ();
   int                                          getMashId         () const;

   // ⮜⮜⮜ All below added for BeerJSON support ⮞⮞⮞
   std::shared_ptr<Boil        >                boil              () const;
   //! \brief This will create a \c Boil object if it doesn't exist
   std::shared_ptr<Boil>                        nonOptBoil        ();
   int                                          getBoilId         () const;
   std::shared_ptr<Fermentation>                fermentation      () const;
   //! \brief This will create a \c Fermentation object if it doesn't exist
   std::shared_ptr<Fermentation>                nonOptFermentation();
   int                                          getFermentationId () const;

   /**
    * \brief This is used in the class implementation and in \c RecipeAttributeButtonBase to convert a Mash, Boil,
    *        Fermentation, Equipment, etc class to its corresponding Recipe property name.
    */
   template<class NE> static BtStringConst const & propertyNameFor();

   /**
    * \brief This is used in \c RecipeAttributeButtonBase and also in places such as \c EditorWithRecipeBase where we
    *        want to do templated code for Mash, Boil, Fermentation, etc.
    *
    *        No general implementation.  Only specialisations, all defined in model/Recipe.cpp.
    */
   template<class NE> std::shared_ptr<NE> get() const;

   int getAncestorId () const;

   // Relational setters
   void setEquipment   (std::shared_ptr<Equipment   > val);
   void setStyle       (std::shared_ptr<Style       > val);
   void setMash        (std::shared_ptr<Mash        > val);
   // ⮜⮜⮜ All below added for BeerJSON support ⮞⮞⮞
   void setBoil        (std::shared_ptr<Boil        > val);
   void setFermentation(std::shared_ptr<Fermentation> val);
   template<typename RA> void setAdditions(QList<std::shared_ptr<RA>> val);
   void setFermentableAdditions(QList<std::shared_ptr<RecipeAdditionFermentable>> val);
   void setHopAdditions        (QList<std::shared_ptr<RecipeAdditionHop        >> val);
   void setMiscAdditions       (QList<std::shared_ptr<RecipeAdditionMisc       >> val);
   void setYeastAdditions      (QList<std::shared_ptr<RecipeAdditionYeast      >> val);
   void setSaltAdjustments     (QList<std::shared_ptr<RecipeAdjustmentSalt     >> val);
   void setWaterUses           (QList<std::shared_ptr<RecipeUseOfWater         >> val);

   /**
    * \brief These calls are intended for use by the ObjectStore when pulling data from the database.  As such they do
    *        not do additional work (eg to ensure that an ingredient being added is a child).
    */
   //! @{
   void setEquipmentId   (int const id);
   void setStyleId       (int const id);
   void setMashId        (int const id);
   // ⮜⮜⮜ Next two added for BeerJSON support ⮞⮞⮞
   void setBoilId        (int const id);
   void setFermentationId(int const id);
   void setAncestorId    (int ancestorId, bool notify = true);
   //! @}

   // Other junk.
   bool hasAncestors() const;
   bool isMyAncestor(Recipe const & maybe) const;
   bool hasDescendants() const;

   // Helpers
   //! \brief Get the IBUs from a given \c hop addition.
   double ibuFromHopAddition(RecipeAdditionHop const & hop);
   // .:TBD:. Not sure reagents is the best word here...
   //! \brief Formats the fermentable additions for instructions
   QList<QString> getReagents(QList<std::shared_ptr<RecipeAdditionFermentable>> fermentableAdditions);
   //! \brief Formats the hops for instructions
   QList<QString> getReagents(QList<std::shared_ptr<RecipeAdditionHop>> hopAdditions, bool firstWort);
   //! \brief Formats the mashsteps for instructions
   QList<QString> getReagents(QList< std::shared_ptr<MashStep> >);

   struct Sugars {
      double sugar_kg_ignoreEfficiency = 0.0;
      double sugar_kg                  = 0.0;
      double nonFermentableSugars_kg   = 0.0;
      double lateAddition_kg           = 0.0;
      double lateAddition_kg_ignoreEff = 0.0;
   };

   Sugars calcTotalPoints();

   // Setters that are not slots
   void setType              (Type    const   val);
   void setBrewer            (QString const & val);
   void setBatchSize_l       (double  const   val);
   void setEfficiency_pct    (double  const   val);
   void setAsstBrewer        (QString const & val);
   void setNotes             (QString const & val);
   void setTasteNotes        (QString const & val);
   void setTasteRating       (double  const   val);
   void setOg                (double  const   val);
   void setFg                (double  const   val);
   void setFermentationStages(int     const   val);
   void setAge_days          (std::optional<double>  const   val);
   void setAgeTemp_c         (std::optional<double>  const   val);
   void setDate              (std::optional<QDate>   const   val);
   void setCarbonation_vols  (std::optional<double>  const   val);
   void setForcedCarbonation (bool    const   val);
   void setPrimingSugarName  (QString const & val);
   void setCarbonationTemp_c (double  const   val);
   void setPrimingSugarEquiv (double  const   val);
   void setKegPrimingFactor  (double  const   val);
   void setLocked            (bool    const   val);
   void setCalcsEnabled      (bool    const   val);
   void setHasDescendants    (bool    const   val);
   // ⮜⮜⮜ All below added for BeerJSON support ⮞⮞⮞
   void setBeerAcidity_pH         (std::optional<double> const val);
   void setApparentAttenuation_pct(std::optional<double> const val);

   /**
    * \brief A Recipe owns some of its contained objects, so needs to delete those if it itself is being deleted
    */
   virtual void hardDeleteOwnedEntities() override;

   /**
    * \brief Deleting a Recipe usually results in an orphaned Mash record (which cannot be removed by
    *        \c hardDeleteOwnedEntities because of the direction of foreign key constraints) and needs to be deleted
    *        immediately after the Recipe record has been removed from the database.
    */
   virtual void hardDeleteOrphanedEntities() override;

   /**
    * \brief This does all the work for the acceptChangeToXxxxx functions below (which can't be templated because the Qt
    *        MOC does not allow it).
    */
   template<class Owned> void acceptChange(QMetaProperty prop, QVariant val);

signals:
   //! Emitted when the number (or order) of instructions changes, or when you should call instructions() again.
   void stepsChanged();

public slots:
   void acceptChangeToContainedObject(QMetaProperty prop, QVariant val);
   // It would be neat if we could template these slots, but the Qt MOC does not allow it, and will give "error:
   // Template function as signal or slot".  These slot functions just call the obvious
   void acceptChangeToRecipeAdditionFermentable(QMetaProperty prop, QVariant val);
   void acceptChangeToRecipeAdditionHop        (QMetaProperty prop, QVariant val);
   void acceptChangeToRecipeAdditionMisc       (QMetaProperty prop, QVariant val);
   void acceptChangeToRecipeAdditionYeast      (QMetaProperty prop, QVariant val);
   void acceptChangeToRecipeAdjustmentSalt     (QMetaProperty prop, QVariant val);
   void acceptChangeToRecipeUseOfWater         (QMetaProperty prop, QVariant val);
   void acceptChangeToBrewNote                 (QMetaProperty prop, QVariant val);
   void acceptChangeToInstruction              (QMetaProperty prop, QVariant val);

protected:
   virtual bool isEqualTo(NamedEntity const & other) const override;
   virtual ObjectStore & getObjectStoreTypedInstance() const override;

private:
   // Private implementation details - see https://herbsutter.com/gotw/_100/
   class impl;
   std::unique_ptr<impl> pimpl;

   // Cached properties that are written directly to db
   Type                  m_type                   ;
   QString               m_brewer                 ;
   QString               m_asstBrewer             ;
   double                m_batchSize_l            ;
   double                m_efficiency_pct         ;
   int                   m_fermentationStages     ;
   std::optional<double> m_age_days               ;
   std::optional<double> m_ageTemp_c              ;
   std::optional<QDate>  m_date                   ;
   std::optional<double> m_carbonation_vols       ;
   bool                  m_forcedCarbonation      ;
   QString               m_primingSugarName       ;
   double                m_carbonationTemp_c      ;
   double                m_primingSugarEquiv      ;
   double                m_kegPrimingFactor       ;
   QString               m_notes                  ;
   QString               m_tasteNotes             ;
   double                m_tasteRating            ;

   int                   m_styleId                ;
   int                   m_equipmentId            ;
   int                   m_mashId                 ;
   // ⮜⮜⮜ All below added for BeerJSON support ⮞⮞⮞
   int                   m_boilId                 ;
   int                   m_fermentationId         ;
   std::optional<double> m_beerAcidity_pH         ;
   std::optional<double> m_apparentAttenuation_pct;

   OwnedSet<Recipe, RecipeAdditionFermentable, PropertyNames::Recipe::fermentableAdditions, &Recipe::acceptChangeToRecipeAdditionFermentable> m_fermentableAdditions;
   OwnedSet<Recipe, RecipeAdditionHop        , PropertyNames::Recipe::hopAdditions        , &Recipe::acceptChangeToRecipeAdditionHop        > m_hopAdditions        ;
   OwnedSet<Recipe, RecipeAdditionMisc       , PropertyNames::Recipe::miscAdditions       , &Recipe::acceptChangeToRecipeAdditionMisc       > m_miscAdditions       ;
   OwnedSet<Recipe, RecipeAdditionYeast      , PropertyNames::Recipe::yeastAdditions      , &Recipe::acceptChangeToRecipeAdditionYeast      > m_yeastAdditions      ;
   OwnedSet<Recipe, RecipeAdjustmentSalt     , PropertyNames::Recipe::saltAdjustments     , &Recipe::acceptChangeToRecipeAdjustmentSalt     > m_saltAdjustments     ;
   OwnedSet<Recipe, RecipeUseOfWater         , PropertyNames::Recipe::waterUses           , &Recipe::acceptChangeToRecipeUseOfWater         > m_waterUses           ;
   /**
    * \brief Each \c BrewNote is a record of a brew day.  We assume that if you're copying a \c Recipe, it is in order
    *        to modify it into a new \c Recipe, in which case it does not make sense to bring the original brew notes
    *        across to the copy.
    */
   OwnedSet<Recipe, BrewNote                 , PropertyNames::Recipe::brewNotes           , &Recipe::acceptChangeToBrewNote,
                                               OwnedSetOptions{.copyable = false}         > m_brewNotes           ;
   /**
    * \brief Instructions are copyable, but they differ from Recipe's other owned sets in being numbered
    */
   OwnedSet<Recipe, Instruction              , PropertyNames::Recipe::instructions        , &Recipe::acceptChangeToInstruction,
                                               OwnedSetOptions{.enumerated = true}        > m_instructions        ;

   /**
    * \brief It's sometimes useful to be able to access the above member variables by type.  This templated member
    *        function allows that.  See Recipe.cpp for specialisations.
    */
   template<class RA> auto & ownedSetFor() const;
   template<class RA> auto & ownedSetFor();

   // Calculated, but stored...BeerXML is weird sometimes.
   // These are mutable because updates to these cached values aren't, in and of themselves, modifications to the
   // recipe.  An update might be triggered by some other change to the recipe or it might be done just because the
   // cached value is stale.
   mutable double m_og;
   mutable double m_fg;

   bool          m_locked      ;
   bool          m_calcsEnabled;

   // True when constructed, indicates whether recalcAll has been called.
   bool                    m_uninitializedCalcs     ;
   QMutex                  m_uninitializedCalcsMutex;
   QMutex                  m_recalcMutex            ;

   // version things
   int                     m_ancestor_id;
   mutable QList<Recipe *> m_ancestors;
   mutable bool            m_hasDescendants;

   // Some recalculators for calculated properties.
   void recalcIfNeeded(QString classNameOfWhatWasAddedOrChanged);

   /**
    * Recalculates all the calculated properties.
    *
    * WARNING: this call took 0.15s in rev 916!
    */
   void recalcAll();
};

// Need specialisations for abstract types
template<> inline Recipe * Recipe::findFirstThatUses([[maybe_unused]] NamedEntity const & var) { return nullptr; }

BT_DECLARE_METATYPES(Recipe)

/**
 * \brief Non-member functions for \c Recipe
 */
namespace RecipeHelper {
   /**
    * \brief Gets the BrewNotes for a Recipe and all its ancestors
    */
   QList<std::shared_ptr<BrewNote>> brewNotesForRecipeAndAncestors(Recipe const & recipe);

   /**
    * \brief Turn automatic versioning on or off
    */
   void setAutomaticVersioningEnabled(bool enabled);

   /**
    * \brief Returns \c true if automatic versioning is enabled, \c false otherwise
    */
   bool getAutomaticVersioningEnabled();

   /**
    * \brief Checks whether an about-to-be-made property change require us to create a new version of a Recipe - eg
    *        because we are modifying some ingredient or other attribute of the Recipe and automatic versioning is
    *        enabled.
    */
   void prepareForPropertyChange(NamedEntity & ne, BtStringConst const & propertyName);

   /**
    * \brief Mini RAII class that allows automatic Recipe versioning to be suspended for the time that it's in scope
    */
   class SuspendRecipeVersioning {
   public:
      SuspendRecipeVersioning();
      ~SuspendRecipeVersioning();
   private:
      bool savedVersioningValue;
      // RAII class shouldn't be getting copied or moved
      SuspendRecipeVersioning(SuspendRecipeVersioning const &) = delete;
      SuspendRecipeVersioning & operator=(SuspendRecipeVersioning const &) = delete;
      SuspendRecipeVersioning(SuspendRecipeVersioning &&) = delete;
      SuspendRecipeVersioning & operator=(SuspendRecipeVersioning &&) = delete;
   };

}
#endif
