/*
 * @author  2009 Pavel Kral
 * @category   C++
 *
 * For the full copyright and license information, please view
 * the file license.txt that was distributed with this source code.
 */

class Receiver : public IEventReceiver
{
public:
       
       Receiver(scene::ISceneNode* nodeW) : node (nodeW){

        }

      // mouse state
		struct SMouseState
        {
                core::position2di Position;
                bool LeftButtonDown;
                SMouseState() : LeftButtonDown(false) { 
				}
        } 
		MouseState;

		 const SMouseState & GetMouseState(void) const
        {
                return MouseState;
        }


        virtual bool OnEvent(const SEvent& event){
                // Remember the mouse state
                if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
                {
                        switch(event.MouseInput.Event)
                        {
                        case EMIE_LMOUSE_PRESSED_DOWN:
                                MouseState.LeftButtonDown = true;
                                break;

                        case EMIE_LMOUSE_LEFT_UP:
                                MouseState.LeftButtonDown = false;
                                break;

                        default:
                                // We won't use the wheel
                                break;
                        }
                }
				if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown)
                {
                        switch (event.KeyInput.Key)
                        {
                        case irr::KEY_KEY_Q: // switch wire frame mode
                                //node->setMaterialFlag(video::EMF_WIREFRAME, true);
								exit(0);
                                return true;
						 case irr::KEY_KEY_C: // switch wire frame mode
								node->setRotation(core::vector3df(0,90,0)); 
					
								return true;
                    
                        default:
                                break;
                        }
                }

                return false;
        }

  private:
	  scene::ISceneNode *node;



};