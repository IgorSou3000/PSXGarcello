#include "gcld1.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"
#include "../timer.h"

//Garcello Death character structure
enum
{
	Gcld1_ArcMain_Idle0,
	Gcld1_ArcMain_Idle1,
	Gcld1_ArcMain_Left0,
	Gcld1_ArcMain_Left1,
	Gcld1_ArcMain_Down0,
    Gcld1_ArcMain_Down1,
	Gcld1_ArcMain_Up0,
	Gcld1_ArcMain_Up1,
	Gcld1_ArcMain_Right0,
	Gcld1_ArcMain_Right1,
	Gcld1_ArcMain_Tight0,
	Gcld1_ArcMain_Tight1,
	Gcld1_ArcMain_Tight2,
	Gcld1_ArcMain_Tight3,
	
	Gcld1_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Gcld1_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Gcld1;
    


//Garcello Death 1 character definitions
static const CharFrame char_Gcld1_frame[] = {
	{Gcld1_ArcMain_Idle0, {  0,   0,   93, 164}, { 48, 155}}, //0 idle 1
	{Gcld1_ArcMain_Idle0, { 93,   0,   94, 164}, { 50, 155}}, //1 idle 2
	{Gcld1_ArcMain_Idle1, {  0,   0,   94, 164}, { 44, 158}}, //2 idle 3
	{Gcld1_ArcMain_Idle1, { 94,   0,   94, 164}, { 51, 158}}, //3 idle 4
	
	{Gcld1_ArcMain_Left0, {  0,   0,  94,166}, { 54,  155}}, //4 left 1
	{Gcld1_ArcMain_Left0, { 94,   0,  94,166}, { 64,  155}}, //5 left 2
	{Gcld1_ArcMain_Left1, {  0,   0,  93,167}, { 54,  155}}, //4 left 1
	{Gcld1_ArcMain_Left1, { 93,   0,  94,167}, { 54,  155}}, //5 left 2
	
	{Gcld1_ArcMain_Down0, {  0,   0, 103, 152}, { 48, 137}}, //6 down 1
	{Gcld1_ArcMain_Down0, {103,   0, 103, 152}, { 48, 137}}, //7 down 2
	{Gcld1_ArcMain_Down1, {  0,   0, 105, 152}, { 48, 138}}, //6 down 1
	{Gcld1_ArcMain_Down1, {105,   0, 105, 152}, { 44, 137}}, //7 down 2
	
	{Gcld1_ArcMain_Up0, {  0,   0,  93, 175}, { 48, 161}}, //8 up 1
	{Gcld1_ArcMain_Up0, { 93,   0,  95, 175}, { 51, 161}}, //9 up 2
	{Gcld1_ArcMain_Up1, {  0,   0,  92, 176}, { 48, 161}}, //8 up 1
	{Gcld1_ArcMain_Up1, { 92,   0,  98, 176}, { 48, 161}}, //9 up 2

	{Gcld1_ArcMain_Right0, {  0,   0, 104, 166}, { 48, 155}}, //10 right 1
	{Gcld1_ArcMain_Right0, {104,   0, 104, 166}, { 48, 155}}, //11 right 2
	{Gcld1_ArcMain_Right1, {  0,   0, 102, 166}, { 48, 155}}, //10 right 1
	{Gcld1_ArcMain_Right1, {102,   0, 102, 166}, { 48, 155}}, //11 right 2

	{Gcld1_ArcMain_Tight0, {  0,   0, 110, 170}, { 48, 155}}, //10 Tight 1
	{Gcld1_ArcMain_Tight0, {111,   0, 110, 170}, { 52, 155}}, //11 Tight 2
	{Gcld1_ArcMain_Tight1, {  0,   0, 112, 172}, { 51, 164}}, //10 Tight 1
	{Gcld1_ArcMain_Tight1, {112,   0, 112, 172}, { 54, 164}}, //11 Tight 2

	{Gcld1_ArcMain_Tight2, {  0,   0, 112, 172}, { 51, 157}}, //10 Tight 1
	{Gcld1_ArcMain_Tight2, {112,   0, 112, 172}, { 54, 157}}, //11 Tight 2
	{Gcld1_ArcMain_Tight3, {  0,   0, 112, 167}, { 50, 157}}, //10 Tight 1
	{Gcld1_ArcMain_Tight3, {112,   0, 112, 167}, { 54, 158}}, //11 Tight 2
};

static const Animation char_gcld1_anim[CharAnim_Max] = {
	{1, (const u8[]){ 1,  1,  2,  2,  3,  3,  0,  0, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){ 4,  5,  6,  7, ASCR_BACK, 1}},   //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},     //CharAnim_LeftAlt
	{2, (const u8[]){ 8,  9, 10, 11, ASCR_BACK, 1}},   //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},     //CharAnim_DownAlt
	{2, (const u8[]){12, 13, 14, 15, ASCR_BACK,  1}},  //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},     //CharAnim_UpAlt
	{2, (const u8[]){16, 17, 18, 19,ASCR_BACK,   1}},  //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},     //CharAnim_RightAlt
	{2, (const u8[]){22,  20,  21,  22,  23,  24,  25,  26,  27,  26,  27, ASCR_BACK, 1}}, //CharAnim_Tight
                                
};

//Gcld1 character functions
void Char_Gcld1_SetFrame(void *user, u8 frame)
{
	Char_Gcld1 *this = (Char_Gcld1*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_Gcld1_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Gcld1_Tick(Character *character)
{
	Char_Gcld1 *this = (Char_Gcld1*)character;
	
	//Perform idle dance
	Character_CheckEndSing(character);
	

	if (stage.flag & STAGE_FLAG_JUST_STEP)
	{
		if ((Animatable_Ended(&character->animatable) || character->animatable.anim == CharAnim_LeftAlt || character->animatable.anim == CharAnim_RightAlt) &&
		    (character->animatable.anim != CharAnim_Left &&
		     character->animatable.anim != CharAnim_Down &&
		     character->animatable.anim != CharAnim_Up &&
		     character->animatable.anim != CharAnim_Right) &&
		    (stage.song_step & 0x3) == 0)
			character->set_anim(character, CharAnim_Idle);

	//Stage specific animations
		if (stage.note_scroll >= 0)
		{
			switch (stage.stage_id)
			{
				case StageId_8_3: //Tight Bars 
					if ((stage.song_step) == 838)
						character->set_anim(character, CharAnim_Tight);
				default:
					break;
			}
		}
	}

	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Gcld1_SetFrame);
	Character_Draw(character, &this->tex, &char_Gcld1_frame[this->frame]);
}
void Char_Gcld1_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Gcld1_Free(Character *character)
{
	Char_Gcld1 *this = (Char_Gcld1*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Gcld1_New(fixed_t x, fixed_t y)
{
	//Allocate Gcld1 object
	Char_Gcld1 *this = Mem_Alloc(sizeof(Char_Gcld1));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Gcld1_New] Failed to allocate Gcld1 object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Gcld1_Tick;
	this->character.set_anim = Char_Gcld1_SetAnim;
	this->character.free = Char_Gcld1_Free;
	
	Animatable_Init(&this->character.animatable, char_gcld1_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character stage information
	this->character.health_i = 13;
	
	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-125,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\GCLD1.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim", //Gcld1_ArcMain_Idle0
		"idle1.tim", //Gcld1_ArcMain_Idle1
		"left0.tim",  //Gcld1_ArcMain_Left
		"left1.tim",  //Gcld1_ArcMain_Left
		"down0.tim",  //Gcld1_ArcMain_Down
		"down1.tim",  //Gcld1_ArcMain_Down
		"up0.tim",    //Gcld1_ArcMain_Up
		"up1.tim",    //Gcld1_ArcMain_Up
		"right0.tim", //Gcld1_ArcMain_Right
		"right1.tim", //Gcld1_ArcMain_Right
		"tight0.tim", //Gcld1_ArcMain_Right
		"tight1.tim", //Gcld1_ArcMain_Right
		"tight2.tim", //Gcld1_ArcMain_Right
		"tight3.tim", //Gcld1_ArcMain_Right
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);

	

	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}