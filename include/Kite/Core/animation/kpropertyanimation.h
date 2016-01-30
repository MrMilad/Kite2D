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
#ifndef KPROPERTYANIMATION_H
#define KPROPERTYANIMATION_H

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/system/kobject.h"
#include "Kite/Core/system/krefvariant.h"
#include "Kite/Core/animation/kanimationbase.h"
#include "Kite/Core/animation/kanimationtypes.h"
#include <string>
#include <vector>

/*! \namespace Kite
\brief Public namespace.
*/
namespace Kite{
	class KITE_FUNC_EXPORT KPropertyAnimation : public KAnimationBase{
	public:
		//! Default constructor
		KPropertyAnimation();
		
		KPropertyAnimation(KObject *Object, const std::string &PropertyName);

		void setTargetObject(KObject *Object);

		void setPropertyName(const std::string &PropertyName);

		inline const std::string &getPropertyName() const { return _kpname; }

		inline const KObject *getTargetObject() const { return _ktarget; }

		void setStartValue(KRefVariant &Value);

		void setEndValue(KRefVariant &Value);

		inline const KRefVariant *getStartValue() const { return _kstartVal; }

		inline const KRefVariant *getEndValue() const { return _kendVal; }

		void setDuration(F32 Duration);

		inline F32 getDuration() const { return _kduration; }

		void setKeyValues(const KAnimKeys *Keys);

		inline const KAnimKeys *getKeyValues() const { return _kkeys; }

		//! Get current playback time in seconds
		inline F32 getCurrentTime() const { return _ktime; }

		//! add a time trigger (if StartTime <= current time <= StartTime + Duration) then trigger
		/*!
			the functor will be executed only once in their duration.
			time count as seconds.
			avoid heavy work in the functor.

			\param Functor Functor
			\param Sender Sender will pass to the functor
			\param StartTime Start time of the trigger
			\param Duration Duration of the trigger
		*/
		void addTimeTrigger(KCallAnimeTrigger Functor, void *Sender, F32 StartTime, F32 Duration);

		//! Set callback for key-change event
		/*!
			the functor will be executed only once per key.
			key-change happens before any trigger.
			normally, the animation played from first key (clip[0]),
			so key-change for the first key in the first turn will not happen.
			avoid heavy work in the functor.

			\param Functor Functor
			\param Sender Sender will pass to the functor
		*/
		void setKeyChangeCallback(KCallKeyChange Functor, void *Sender);

		//! Delete all time triggers
		void deleteAllTimeTriggers();

	private:
		void triggerLookup(); // lookup triggers
		void keyLookup(); // lookup keys and find appropriate key (depend of time)

		std::vector<Internal::KAnimeTimeTrigger> _kttrig;
		F64 _ktime;

		std::vector<KAnimeObject *> _klist;
		KAnimeValue _kvalue;
		U32 _kcurrentKey;
		
		KCallKeyChange _kkcall;
		void *_kksender;

		F32 _kduration;
		KAnimKeys *_kkeys;
		KRefVariant *_kstartVal;
		KRefVariant *_kendVal;
		std::string _kpname;
		KObject *_ktarget;
	};
}

#endif // KPROPERTYANIMATION_H
