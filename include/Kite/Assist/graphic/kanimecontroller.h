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
#ifndef KANIMECONTROLLER_H
#define KANIMECONTROLLER_H

/*! \file kanimecontroller.h */

#include "Kite/Core/system/ksystemdef.h"
#include "Kite/Core/graphic/kgraphicstructs.h"
#include "Kite/Core/graphic/kgraphictypes.h"
#include "Kite/Assist/graphic/kanimeobject.h"
#include <vector>

/*! \namespace Kite
\brief Public namespace.
*/
namespace Kite{

	//! The KAnimeController class is a utility class for running key based animations.
	/*!
		This class can play and manipulate animation clips (vector of the KAnimKeys).
		for sprite-sheet animation in loop mode, always add an extra key at the end of the clip to avoiding (first and last) frame overlap.
	*/
	class KITE_FUNC_EXPORT KAnimeController{
	public:
		//! Default constructor
		KAnimeController();

		//! Constructor the controller with one object, one clip and one optional sprite-sheet.
		/*!
			\param Object Object
			\param AnimationClip Animation clip
			\param SpriteSheet Sprite-sheet (optional - pass 0 if there is no sprite sheet)
		*/
		KAnimeController(KAnimeObject *Object, const std::vector<KAnimeKey> *AnimationClip, const std::vector<KAtlas> *SpriteSheet);

		//! Add a object to list
		/*!
			all object in the list Will be animated in parallel

			\param Object Object
		*/
		void addObject(KAnimeObject *Object);

		//! Clear object list
		void deleteAllObjects();

		//! Set animation clip
		/*!
			\param AnimationClip Animation clip
		*/
		void setClip(const std::vector<KAnimeKey> *AnimationClip);

		//! Get animation clip
		inline const std::vector<KAnimeKey> *getClip() const { return _kclip; }

		//! Set sprite-sheet
		/*!
			\param SpriteSheet Sprite-sheet (pass 0 if there is no sprite sheet)
		*/
		inline void setSpriteSheet(const std::vector<KAtlas> *SpriteSheet) { _katlas = SpriteSheet; }

		//! Get sprite-sheet
		inline const std::vector<KAtlas> *getSpriteSheet() const { return _katlas; }

		//! Set animation loop
		/*!
			loop apply at any direction (foreward and backwrad)

			\param Loop Animation loop
		*/
		inline void setLoop(bool Loop) { _kloop = Loop; }

		//! Get animation loop
		inline bool getLoop() const { return _kloop; }

		//! Set animation playback direction
		/*!
			\param Direction Animation playback direction
		*/
		inline void setPlayDirection(KAnimeDirectionTypes Direction) { _kdtype = Direction; }

		//! Get animation playback direction
		inline KAnimeDirectionTypes getPlayDirection() const { return _kdtype; }

		//! Set the animation playback state
		/*!
			\param State Animation playback state
		*/
		inline void setState(KAnimeStateTypes State) { _kstype = State; }

		//! Get the animation playback state
		inline KAnimeStateTypes getState() const { return _kstype; }

		//! Set current playback time in seconds
		/*!
			Time must be in range (0 to last key.time)

			\param Time Current playback time
		*/
		void setCurrentTime(F32 Time);

		//! Get current playback time in seconds
		inline F64 getCurrentTime() const { return _ktime; }

		//! Get total time of the animation clip
		F32 getTotalTime() const;

		//! Set current key of the animation clip
		/*!
			KeyIndex must be in range (0 to clip size() - 1) 

			\param KeyIndex Current key of the animation clip
		*/
		void setCurrentKey(U32 KeyIndex);

		//! Get current key of the animation clip
		inline U32 getCurrentKey() const { return _kcurrentKey; }

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

		//! Update animation state
		/*!
			Positive and negative values as Delta, are ineffective in play direction (use setPlayDirection())

			\param Delta Delta time 
		*/
		void update(F32 Delta);

	private:
		void triggerLookup(); // lookup triggers
		void keyLookup(); // lookup keys and find appropriate key (depend of time)
		const std::vector<KAnimeKey> *_kclip;
		const std::vector<KAtlas> *_katlas;
		std::vector<Internal::KAnimeTimeTrigger> _kttrig;
		F64 _ktime;
		bool _kloop;
		KAnimeStateTypes _kstype;
		std::vector<KAnimeObject *> _klist;
		KAnimeValue _kvalue;
		U32 _kcurrentKey;
		KAnimeDirectionTypes _kdtype;
		KCallKeyChange _kkcall;
		void *_kksender;
	};
}

#endif // KANIMECONTROLLER_H
