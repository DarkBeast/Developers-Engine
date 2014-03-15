/*******************************************************************************
* Credits:  Andrew Wheeler/Genusis
*           Jacob/Dugar
******************************************************************************/

#ifndef DE_PACKETENUM_H
#define DE_PACKETENUM_H

typedef enum serverpackets serverpackets;
typedef enum clientpackets clientpackets;

//define the client packet ids so we know where they go.
enum clientpackets{
	CNONE = 0,
	CLOGIN,//always keep first = 1.
	CNEWACCOUNT,
	CSAYMSG,
	CEMOTEMSG,
	CBROADCASTMSG,
	CGLOBALMSG,
	CADMINMSG,
	CPLAYERMSG,
	CPLAYERMOVE,
	CPLAYERDIR,
	CUSEITEM,
	CATTACK,
	CUSESTATPOINT,
	CWARPMETO,
	CWARPTOME,
	CWARPTO,
	CSETSPRITE,
	CREQUESTNEWMAP,
	CMAPDATA,
	CNEEDMAP,
	CMAPGETITEM,
	CMAPDROPITEM,
	CMAPRESPAWN,
	CKICKPLAYER,
	CBANPLAYER,
	CREQUESTEDITMAP,
	CREQUESTEDITITEM,
	CEDITITEM,
	CSAVEITEM,
	CREQUESTEDITNPC,
	CEDITNPC,
	CSAVENPC,
	CREQUESTEDITSHOP,
	CEDITSHOP,
	CSAVESHOP,
	CREQUESTEDITSPELL,
	CEDITSPELL,
	CSAVESPELL,
	CDELETE,
	CSETGROUP,
	CWHOSONLINE,
	CTRADE,
	CTRADEREQUEST,
	CFIXITEM,
	CSEARCH,
	CSPELLS,
	CCAST,
	CQUIT,
	CMSG_COUNT //always keep last
};

//define the server packet ids so we know where they go.
enum serverpackets{
	SNONE = 0,
	SLOGINOK,
	SSTATUS,
	SALERTMSG,
	SINGAME,
	SPLAYERINV,
	SPLAYERINVUPDATE,
	SPLAYERWORNEQ,
	SPLAYERHP,
	SPLAYERMP,
	SPLAYERSP,
	SPLAYERSTATS,
	SPLAYERDATA,
	SPLAYERMOVE,
	SNPCMOVE,
	SPLAYERDIR,
	SNPCDIR,
	SPLAYERXY,
	SATTACK,
	SNPCATTACK,
	SCHECKFORMAP,
	SMAPDATA,
	SMAPITEMDATA,
	SMAPNPCDATA,
	SMAPDONE,
	SGLOBALMSG,
	SADMINMSG,
	SPLAYERMSG,
	SMAPMSG,
	SSPAWNITEM,
	SITEMEDITOR,
	SUPDATEITEM,
	SEDITITEM,
	SREDITOR,
	SSPAWNNPC,
	SNPCDEAD,
	SNPCEDITOR,
	SUPDATENPC,
	SEDITNPC,
	SEDITMAP,
	SSHOPEDITOR,
	SUPDATESHOP,
	SEDITSHOP,
	SSPELLEDITOR,
	SUPDATESPELL,
	SEDITSPELL,
	STRADE,
	SSPELLS,
	SLEFT,
	SCASTSPELL,
	SHIGHINDEX,
	SMSG_COUNT//always keep last
};

#endif