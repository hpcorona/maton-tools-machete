
#include "default.h"
#include "../draw/asha.h"

namespace machete {
	namespace input {
		
		void SingleTouchHandler(void* data, int id, int event, float x, float y) {
		  machete::input::TheTouchInput->SetTouchCount(1);
		  machete::input::Touch *t = machete::input::TheTouchInput->GetTouch(0);

		  if (event == machete::input::TouchStart) {
		    t->current.x = x;
		    t->current.y = y;
		    machete::draw::TheAshaManager->AdaptPosition(t->current);
		    t->previous = t->current;
		    t->start = t->current;
		    t->movement.x = 0;
		    t->movement.y = 0;
		    t->nextPhase = machete::input::TouchNone;

		    t->offset.x = 0;
		    t->offset.y = 0;
		    t->finger = id;
		    t->phase = (machete::input::TouchPhase)event;

		    t->owner = NULL;

		    t->consumed = false;
		  } else {
		    if (!t->consumed && t->phase == machete::input::TouchStart) {
		      Vec2 lagPos(x, y);
		      machete::draw::TheAshaManager->AdaptPosition(lagPos);

		      t->movement += lagPos - t->current;
		      t->nextPhase = (machete::input::TouchPhase)event;
		      t->consumed = false;
		      t->finger = id;
		    } else {
					if (!t->consumed) {
						Vec2 lagPos(x, y);
						machete::draw::TheAshaManager->AdaptPosition(lagPos);

						t->movement += lagPos - t->current;
						t->nextPhase = (machete::input::TouchPhase)event;
						t->consumed = false;
						t->finger = id;
					} else {
						t->previous = t->current;
						t->current.x = x;
						t->current.y = y;
						machete::draw::TheAshaManager->AdaptPosition(t->current);

						t->movement.x = 0;
						t->movement.y = 0;
						t->nextPhase = machete::input::TouchNone;
						t->offset = t->current - t->previous;
						t->finger = id;
						t->phase = (machete::input::TouchPhase)event;
						t->consumed = false;
					}
		    }
		  }

		  machete::input::TheTouchInput->MarkAvailable();
		}
		
	}
}
