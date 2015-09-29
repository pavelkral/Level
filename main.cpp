/*
 * @author  2009 Pavel Kral
 * @category   C++
 *
 * For the full copyright and license information, please view
 * the file license.txt that was distributed with this source code.
 */

#include <irrlicht.h>
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#endif

#include "receiver.h"

enum{
	ID_IsNotPickable = 0,
	IDFlag_IsPickable = 1 << 0,
	IDFlag_IsHighlightable = 1 << 1
};

int main()
{
	  int width = 800;
	  int height = 600;
	  bool window = false;
/*/
	printf("Please select resolution :\n"\
		" (a) 800x600 window\n (b) 1440x1050 fullscreen\n (c) 1366x768 fullscreen\n (d) 1680x1050 fullscreen\n \n");

	  char i;
	  
	  std::cin >> i;

	  switch(i)
	  {
	  case 'a': 
		 width = 800;
		 height = 600;
		 window = false;

		  break;
	  case 'b': 
		 width = 1440;
		 height = 1050;
		 window = true;

		  break;
	case 'c': 
		 width = 1366;
		 height = 768;
		 window = true;

		  break;
	  case 'd':
		 width = 1680;
		 height = 1050;
		 window = true;

		  break;

	  default: return 0;
	  }	

/*/

//...................................................................Main Device.........................................................

	
	IrrlichtDevice *device = createDevice(video::EDT_DIRECT3D9, core::dimension2d<u32>(width, height), 32, window,true);
	device->getCursorControl()->setVisible(false);
	video::IVideoDriver* driver = device->getVideoDriver();

	scene::ISceneManager* smgr = device->getSceneManager();
	smgr->setShadowColor(video::SColor(100,0,0,0));

//....................................................................Gui.........................................................


	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
	gui::IGUIImage* img = guienv->addImage(driver->getTexture("media/x.png"),position2d<int>(10,10));
	guienv->getSkin()->setFont(guienv->getFont("media/fonthaettenschweiler.bmp"));

   	gui::IGUIStaticText* diagnostics = guienv->addStaticText(
                L"Press 'Right mouse' to change wireframe mode\nPress 'Q' to quit",
                core::rect<s32>(width-270,10,width-10,40), true, true, 0, -1, true);
        diagnostics->setOverrideColor(video::SColor(255, 255, 255, 255));


//..................................................................World and camera.........................................................

	IMetaTriangleSelector *Meta = smgr->createMetaTriangleSelector();


	scene::IAnimatedMesh* mesh2 = smgr->getMesh("media/world.x");
	scene::IAnimatedMeshSceneNode* node2 = smgr->addAnimatedMeshSceneNode( mesh2 );
	node2->setPosition(core::vector3df(0,0,0)); 
	
	scene::ITriangleSelector* selector = 0;
	selector = smgr->createOctreeTriangleSelector(node2->getMesh(), node2, 64);
	Meta->addTriangleSelector( selector);
	node2->setTriangleSelector(selector);
	selector->drop(); 


	SKeyMap keyMap[9];
	keyMap[2].Action = EKA_MOVE_BACKWARD;
	keyMap[2].KeyCode = KEY_DOWN;
	keyMap[3].Action = EKA_MOVE_BACKWARD;
	keyMap[3].KeyCode = KEY_KEY_S;
	keyMap[4].Action = EKA_STRAFE_LEFT;
	keyMap[4].KeyCode = KEY_LEFT;
	keyMap[5].Action = EKA_STRAFE_LEFT;
	keyMap[5].KeyCode = KEY_KEY_A;
	keyMap[6].Action = EKA_STRAFE_RIGHT;
	keyMap[6].KeyCode = KEY_RIGHT;
	keyMap[7].Action = EKA_STRAFE_RIGHT;
	keyMap[7].KeyCode = KEY_KEY_D;
	keyMap[0].Action = EKA_MOVE_FORWARD;
	keyMap[0].KeyCode = KEY_UP;
	keyMap[1].Action = EKA_MOVE_FORWARD;
	keyMap[1].KeyCode = KEY_KEY_W;
	keyMap[8].Action = EKA_JUMP_UP;
	keyMap[8].KeyCode = KEY_SPACE;


	scene::ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS(0, 100, .3f, ID_IsNotPickable, keyMap, 9,false,3.f);
//	scene::ICameraSceneNode* camera =  smgr->addCameraSceneNodeFPS(0, 100.0f, .3f, ID_IsNotPickable, 0, 0, true, 3.f);
	camera->setPosition(core::vector3df(50,120,-90));
	camera->setTarget(core::vector3df(0,120,0));
  
	IAnimatedMeshMD2* weaponMesh = (IAnimatedMeshMD2*) smgr->getMesh("media/gun.md2");
	if ( 0 == weaponMesh ){
                return 1;
	}
        if ( weaponMesh->getMeshType() == EAMT_MD2 )
        {
                s32 count = weaponMesh->getAnimationCount();
                for ( s32 i = 0; i != count; ++i )
                {
                       // snprintf ( buf, 64, "Animation: %s", weaponMesh->getAnimationName(i) );
                      //  device->getLogger()->log(buf, ELL_INFORMATION);
                }
        }

	IAnimatedMeshSceneNode* WeaponNode = smgr->addAnimatedMeshSceneNode(weaponMesh,smgr->getActiveCamera(),10,vector3df( 0, 0, 0),vector3df(-90,-90,90));
	WeaponNode->setMaterialFlag(EMF_LIGHTING, false);
	WeaponNode->setMaterialTexture(0, driver->getTexture( "media/gun.jpg"));
	WeaponNode->setLoopMode ( true );
	WeaponNode->setName ( "gun" );

      

	scene::IBillboardSceneNode * bill = smgr->addBillboardSceneNode();
	bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
	bill->setMaterialTexture(0, driver->getTexture("media/particle.bmp"));
	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialFlag(video::EMF_ZBUFFER, false);
	bill->setSize(core::dimension2d<f32>(20.0f, 20.0f));
	bill->setID(ID_IsNotPickable); 


	
	
//............................................................Colision models................................................................
	   


	scene::IAnimatedMeshSceneNode* nodeW = 0;
	nodeW = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/anmarine.x"),
                                                0, IDFlag_IsPickable | IDFlag_IsHighlightable);
	nodeW->setPosition(core::vector3df(300,20,-120)); 
	nodeW->setRotation(core::vector3df(0,90,0)); 
	nodeW->setAnimationSpeed(20.f);
	nodeW->addShadowVolumeSceneNode();
	nodeW->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

	selector = smgr->createTriangleSelector(nodeW->getMesh(),nodeW);
	Meta->addTriangleSelector( selector);
    nodeW->setTriangleSelector(selector);
    selector->drop();
/*/		

	scene::ISceneNodeAnimator* anim2 =
                        smgr->createFlyStraightAnimator(core::vector3df(0,170,-120),
                        core::vector3df(-1500,170,-120), 5500, true,true);
    if (anim2){
           // nodeW->addAnimator(anim2);
            anim2->drop();
    }

/*/


	IAnimatedMeshSceneNode* marine = 0;
	marine = smgr->addAnimatedMeshSceneNode(smgr->getMesh("media/anmarine.x"),
                                                0, IDFlag_IsPickable | IDFlag_IsHighlightable);
	marine->setPosition(core::vector3df(-1650,20,-150)); 
	marine->setRotation(core::vector3df(0,-90,0)); 
    marine->setAnimationSpeed(30.f);
	marine->addShadowVolumeSceneNode();
    marine->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	

	selector = smgr->createTriangleSelector(marine->getMesh(),marine);
	Meta->addTriangleSelector( selector);
    marine->setTriangleSelector(selector);
    selector->drop();



	scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(Meta, camera, core::vector3df(40,90,40),core::vector3df(0,-10,0), core::vector3df(0,20,0));
	camera->addAnimator(anim);
	Meta->drop();
	anim->drop(); 

 /*/
	 scene::ISceneNodeAnimator* animW = 
	 smgr->createRotationAnimator(core::vector3df(0, 0.8f, 0));
	 nodeW->addAnimator(animW);
	 animW->drop();

	nodeW = smgr->addLightSceneNode(0, core::vector3df(0,0,0),video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 500.0f);
    scene::ISceneNodeAnimator* animws = 0;
    animws = smgr->createFlyCircleAnimator (core::vector3df(-800,50,0),200.0f);
    nodeW->addAnimator(animws);
    animws->drop();

   // attach billboard to light

    nodeW = smgr->addBillboardSceneNode(nodeW, core::dimension2d<f32>(50, 50));
    nodeW->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    nodeW->setMaterialTexture(0, driver->getTexture("media/particlewhite.bmp"));
	/*/

	
//....................................................Dynamic Lights...........................................................................


    scene::ISceneNode * light = smgr->addLightSceneNode(0, core::vector3df(50,220,0),video::SColorf(1.0f,1.0f,1.0f,1.0f), 600.0f);
	light->setID(ID_IsNotPickable); 
    light = smgr->addBillboardSceneNode(light, core::dimension2d<f32>(50, 50));
	light->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    light->setMaterialTexture(0, driver->getTexture("media/particlewhite.bmp"));
    light->setMaterialFlag(video::EMF_LIGHTING, false);

	//right light

	scene::ISceneNode * light2 = smgr->addLightSceneNode(0, core::vector3df(-1200,420,1200),video::SColorf(1.0f,1.0f,1.0f,1.0f), 600.0f);
	light2->setID(ID_IsNotPickable);
	light2 = smgr->addBillboardSceneNode(light2, core::dimension2d<f32>(50, 50));
	light2->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    light2->setMaterialTexture(0, driver->getTexture("media/particlewhite.bmp"));
    light2->setMaterialFlag(video::EMF_LIGHTING, false);

	//left light

	scene::ISceneNode * light3 = smgr->addLightSceneNode(0, core::vector3df(-1200,220,-1200),video::SColorf(1.0f,1.0f,1.0f,1.0f), 600.0f);
	light3->setID(ID_IsNotPickable); 
	light3 = smgr->addBillboardSceneNode(light3, core::dimension2d<f32>(50, 50));
	light3->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    light3->setMaterialTexture(0, driver->getTexture("media/particlewhite.bmp"));
    light3->setMaterialFlag(video::EMF_LIGHTING, false);

	//top light

	scene::ISceneNode * light4 = smgr->addLightSceneNode(0, core::vector3df(-1500,220,0),video::SColorf(1.0f,1.0f,1.0f,1.0f), 600.0f);
	light4->setID(ID_IsNotPickable); 
	light4 = smgr->addBillboardSceneNode(light4, core::dimension2d<f32>(50, 50));
	light4->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    light4->setMaterialTexture(0, driver->getTexture("media/particlewhite.bmp"));
    light4->setMaterialFlag(video::EMF_LIGHTING, false);


//......................................................Next floor Lights.........................................................................
//


	scene::ISceneNode * light5 = smgr->addLightSceneNode(0, core::vector3df(50,490,0),video::SColorf(1.0f,1.0f,1.0f,1.0f), 600.0f);
	light5->setID(ID_IsNotPickable); 
    light5 = smgr->addBillboardSceneNode(light5, core::dimension2d<f32>(50, 50));
	light5->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    light5->setMaterialTexture(0, driver->getTexture("media/particlewhite.bmp"));
    light5->setMaterialFlag(video::EMF_LIGHTING, false);

	//left light

	scene::ISceneNode * light3t = smgr->addLightSceneNode(0, core::vector3df(-1200,490,-1200),video::SColorf(1.0f,1.0f,1.0f,1.0f), 600.0f);
	light3t->setID(ID_IsNotPickable); 
	light3t = smgr->addBillboardSceneNode(light3t, core::dimension2d<f32>(50, 50));
	light3t->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    light3t->setMaterialTexture(0, driver->getTexture("media/particlewhite.bmp"));
    light3t->setMaterialFlag(video::EMF_LIGHTING, false);

	//top light

	scene::ISceneNode * light4t = smgr->addLightSceneNode(0, core::vector3df(-1500,490,0),video::SColorf(1.0f,1.0f,1.0f,1.0f), 600.0f);
	light4t->setID(ID_IsNotPickable); 
	light4t = smgr->addBillboardSceneNode(light4t, core::dimension2d<f32>(50, 50));
	light4t->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    light4t->setMaterialTexture(0, driver->getTexture("media/particlewhite.bmp"));
    light4t->setMaterialFlag(video::EMF_LIGHTING, false);

//

//.......................................................................Main......................................................


	SMaterial material;
	material.setTexture(0, 0);
    material.Lighting = false;
	material.Wireframe=true;
	scene::ISceneCollisionManager* collMan = smgr->getSceneCollisionManager();

	Receiver receiver(nodeW);
    device->setEventReceiver(&receiver);

	gui::IGUIFont* font = guienv->getBuiltInFont();
	 
	int fps2 = 0;
	int lastFPS = -1;


	while(device->run()){

		if (device->isWindowActive())
		{
			driver->beginScene(true, true, 0);

			smgr->drawAll();

			guienv->drawAll();
		
			core::line3d<f32> ray;
			ray.start = camera->getPosition();
			ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 1000.0f;
//
			fps2 = driver->getFPS();
			core::stringw str2 = L"Level2 Demo 0.1 ";
			str2 += " FPS: ";
			str2 += fps2;
			font->draw(str2,core::rect<s32>(50,10,100,60),video::SColor(255,255,255,255));
//
			//driver->setMaterial(material);
			//driver->draw3DLine(core::vector3df(100,30,-300),core::vector3df(100,30,300),video::SColor(0,255,0,0));
			//driver->draw3DLine(camera->getPosition(),core::vector3df(0,90,0),video::SColor(0,255,0,0));

			core::vector3df intersection;
			core::triangle3df hitTriangle;
			
			bill->setPosition(camera->getTarget());
					
			scene::ISceneNode * selectedSceneNode =
			collMan->getSceneNodeAndCollisionPointFromRay(
						ray,
						intersection, // collision
						hitTriangle, // triangle  collision
						IDFlag_IsPickable, // This ensures that only nodes that we have 
						0); // Check the entire scene 
			
				//	ISceneNode * selectedSceneNode = collMan->getSceneNodeFromCameraBB(camera);

				// core::vector3df nodePosition = nodeW->getPosition();			 
				// if(nodePosition.Z >= -300 ){
					 //    nodeW->setRotation(core::vector3df(0,90,0));  
				// }
	
			if(selectedSceneNode){

				if(selectedSceneNode == marine){
					marine->setMaterialFlag(video::EMF_WIREFRAME, receiver.GetMouseState().LeftButtonDown);
					driver->setTransform(video::ETS_WORLD, core::matrix4());
				}
				else{
			
				}
				if(selectedSceneNode == nodeW){
					nodeW->setMaterialFlag(video::EMF_WIREFRAME, receiver.GetMouseState().LeftButtonDown);
					driver->setTransform(video::ETS_WORLD, core::matrix4());
					driver->draw3DTriangle(hitTriangle, video::SColor(0,255,0,0));
					
				}
				else{
				
				}

			}

			driver->endScene();

			int fps = driver->getFPS();

			if (lastFPS != fps){

			  core::stringw str = L"Level demo - Irrlicht Engine [";
			  str += driver->getName();
			  str += "] FPS:";
			  str += fps;
			  device->setWindowCaption(str.c_str());
			  lastFPS = fps;
			}

		}
	}
	device->drop();

	return 0;
}

/*
**/
