/*
 Name:		TestKodesh.ino
 Created:	4/28/2020 5:33:15 AM
 Author:	MCP
*/

#include "Kodesh.h"
#include "Sun.h"

using namespace NYG;

namespace NYG
{
	extern uint8_t gbl_TOSEFET_KNISAT_KODESH;
	extern uint8_t gbl_TOSEFET_TZET_KODESH;

	#define TOSEFET_KNISAT_KODESH_SECONDS		((uint16_t)gbl_TOSEFET_KNISAT_KODESH * SECONDS_PER_MINUTE)
	#define TOSEFET_TZET_KODESH_SECONDS			((uint16_t)gbl_TOSEFET_TZET_KODESH   * SECONDS_PER_MINUTE)
}

// the setup function runs once when you press reset or power the board
void setup() {
	//Kodesh::verify_holidays();

	_LOGGER << DstTime(Sun::GetLocalSetTime(2020, 4,  4)).ToText()	<< NL;
	_LOGGER << DstTime(Sun::GetLocalSetTime(2020, 4, 11)).ToText() << NL;
	_LOGGER << DstTime(Sun::GetLocalSetTime(2020, 4, 18)).ToText() << NL;
	_LOGGER << DstTime(Sun::GetLocalSetTime(2020, 4, 25)).ToText() << NL;
	_LOGGER << DstTime(Sun::GetLocalSetTime(2020, 5,  2)).ToText() << NL;

	TRACING = false;

	#define TEST_HTT_1(dt, tt, step)	{ uint16_t htt = Kodesh::GetHebTimeType(dt); if(htt != (tt)) _LOGGER << __LINE__ << ". " << (dt).ToText() << ": 0x" << RXHEX << htt << " != " << #tt << NL; else { Kodesh::HebTimeStep hs = Kodesh::GetHebTimeStep(dt); if (hs != (Kodesh::step)) _LOGGER << __LINE__ << ". " << (dt).ToText() << ": " << Kodesh::GetHebTimeStepName(hs) << " != " << #step << NL; } }
	#define TEST_HTT_2(tp, tt, step)	TEST_HTT_1(FixTime tp, tt, step)
	
	TEST_HTT_2((2020, 4, 27), Kodesh::Chol,			CHOL);
	TEST_HTT_2((2020, 5,  1), Kodesh::ErevShabat,	EREV_KODESH);

	FixTime skyia = Sun::GetLocalSetTime(FixTime(2020, 5, 1));

	TEST_HTT_1(skyia - (TOSEFET_KNISAT_KODESH_SECONDS + 1),	Kodesh::ErevShabat,		EREV_KODESH);
	TEST_HTT_1(skyia - TOSEFET_KNISAT_KODESH_SECONDS,		Kodesh::AchareiHadlaka, HADLAKA);
	TEST_HTT_1(skyia - 1,									Kodesh::AchareiHadlaka, HADLAKA);
	TEST_HTT_1(skyia,										Kodesh::Shabat,			KODESH);
	TEST_HTT_1(skyia + 1,									Kodesh::Shabat,			KODESH);

	skyia = Sun::GetLocalSetTime(skyia + SECONDS_PER_DAY);

	TEST_HTT_1(skyia - 1,									Kodesh::Shabat,							KODESH);
	TEST_HTT_1(skyia,										Kodesh::Shabat | Kodesh::AchareiShkyia, KODESH);
	TEST_HTT_1(skyia + TOSEFET_TZET_KODESH_SECONDS - 1,		Kodesh::Shabat | Kodesh::AchareiShkyia, KODESH);
	TEST_HTT_1(skyia + TOSEFET_TZET_KODESH_SECONDS,			Kodesh::Chol,							CHOL);

	// PESACH
	TEST_HTT_2((2020, 4, 7), Kodesh::Chol,		CHOL);
	TEST_HTT_2((2020, 4, 8), Kodesh::ErevChag,	EREV_KODESH);

	skyia = Sun::GetLocalSetTime(FixTime(2020, 4, 8));

	TEST_HTT_1(skyia - (TOSEFET_KNISAT_KODESH_SECONDS + 1), Kodesh::ErevChag,			EREV_KODESH);
	TEST_HTT_1(skyia - TOSEFET_KNISAT_KODESH_SECONDS,		Kodesh::AchareiHadlaka,		HADLAKA);
	TEST_HTT_1(skyia - 1,									Kodesh::AchareiHadlaka,		HADLAKA);
	TEST_HTT_1(skyia,										Kodesh::Pesach,				KODESH);
	TEST_HTT_1(skyia + 1,									Kodesh::Pesach,				KODESH);

	skyia = Sun::GetLocalSetTime(skyia + SECONDS_PER_DAY);

	TEST_HTT_1(skyia - 1,									Kodesh::Pesach,							KODESH);
	TEST_HTT_1(skyia,										Kodesh::Pesach | Kodesh::AchareiShkyia, KODESH);
	TEST_HTT_1(skyia + TOSEFET_TZET_KODESH_SECONDS - 1,		Kodesh::Pesach | Kodesh::AchareiShkyia, KODESH);
	TEST_HTT_1(skyia + TOSEFET_TZET_KODESH_SECONDS,			Kodesh::Chol,							CHOL);

	// SHAVUOT + SHABAT

	TEST_HTT_2((2020, 5, 27), Kodesh::Chol,		CHOL);
	TEST_HTT_2((2020, 5, 28), Kodesh::ErevChag, EREV_KODESH);
	
	skyia = Sun::GetLocalSetTime(FixTime(2020, 5, 28));

	TEST_HTT_1(skyia - (TOSEFET_KNISAT_KODESH_SECONDS + 1), Kodesh::ErevChag,				EREV_KODESH);
	TEST_HTT_1(skyia - TOSEFET_KNISAT_KODESH_SECONDS,		Kodesh::AchareiHadlaka,			HADLAKA);
	TEST_HTT_1(skyia - 1,									Kodesh::AchareiHadlaka,			HADLAKA);
	TEST_HTT_1(skyia,										Kodesh::Shavuot,				KODESH);
	TEST_HTT_1(skyia + 1,									Kodesh::Shavuot,				KODESH);

	skyia = Sun::GetLocalSetTime(skyia + SECONDS_PER_DAY);

	TEST_HTT_1(skyia - 1,									Kodesh::Shavuot,					KODESH);
	TEST_HTT_1(skyia,										Kodesh::Shavuot | Kodesh::Shabat,	KODESH);
	TEST_HTT_1(skyia + TOSEFET_TZET_KODESH_SECONDS - 1,		Kodesh::Shavuot | Kodesh::Shabat,	KODESH);
	TEST_HTT_1(skyia + TOSEFET_TZET_KODESH_SECONDS,			Kodesh::Shabat,						KODESH);

	skyia = Sun::GetLocalSetTime(skyia + SECONDS_PER_DAY);

	TEST_HTT_1(skyia - 1,									Kodesh::Shabat,							KODESH);
	TEST_HTT_1(skyia,										Kodesh::Shabat | Kodesh::AchareiShkyia, KODESH);
	TEST_HTT_1(skyia + TOSEFET_TZET_KODESH_SECONDS - 1,		Kodesh::Shabat | Kodesh::AchareiShkyia, KODESH);
	TEST_HTT_1(skyia + TOSEFET_TZET_KODESH_SECONDS,			Kodesh::Chol,							CHOL);

	// ROSH HASHANA

	TEST_HTT_2((2020, 9, 17), Kodesh::Chol,								CHOL);
	TEST_HTT_2((2020, 9, 18), Kodesh::ErevChag | Kodesh::ErevShabat,	EREV_KODESH);

	skyia = Sun::GetLocalSetTime(FixTime(2020, 9, 18));

	TEST_HTT_1(skyia - (TOSEFET_KNISAT_KODESH_SECONDS + 1), Kodesh::ErevChag	 | Kodesh::ErevShabat,	EREV_KODESH);
	TEST_HTT_1(skyia - TOSEFET_KNISAT_KODESH_SECONDS,		Kodesh::AchareiHadlaka,						HADLAKA);
	TEST_HTT_1(skyia - 1,									Kodesh::AchareiHadlaka,						HADLAKA);
	TEST_HTT_1(skyia,										Kodesh::RoshHaShanaA | Kodesh::Shabat,		KODESH);
	TEST_HTT_1(skyia + 1,									Kodesh::RoshHaShanaA | Kodesh::Shabat,		KODESH);

	skyia = Sun::GetLocalSetTime(skyia + SECONDS_PER_DAY);

	TEST_HTT_1(skyia - 1,									Kodesh::RoshHaShanaA | Kodesh::Shabat,		KODESH);
	TEST_HTT_1(skyia,										Kodesh::RoshHaShanaA | Kodesh::Shabat,		KODESH);
	TEST_HTT_1(skyia + TOSEFET_TZET_KODESH_SECONDS - 1,		Kodesh::RoshHaShanaA | Kodesh::Shabat,		KODESH);
	TEST_HTT_1(skyia + TOSEFET_TZET_KODESH_SECONDS,			Kodesh::RoshHaShanaB,						KODESH);
	
	skyia = Sun::GetLocalSetTime(skyia + SECONDS_PER_DAY);

	TEST_HTT_1(skyia - 1,									Kodesh::RoshHaShanaB,							KODESH);
	TEST_HTT_1(skyia,										Kodesh::RoshHaShanaB | Kodesh::AchareiShkyia,	KODESH);
	TEST_HTT_1(skyia + TOSEFET_TZET_KODESH_SECONDS - 1,		Kodesh::RoshHaShanaB | Kodesh::AchareiShkyia,	KODESH);
	TEST_HTT_1(skyia + TOSEFET_TZET_KODESH_SECONDS,			Kodesh::Chol,									CHOL);

	// ROSH HASHANA + SHABAT

	TEST_HTT_2((2024, 10, 1), Kodesh::Chol,		CHOL);
	TEST_HTT_2((2024, 10, 2), Kodesh::ErevChag, EREV_KODESH);

	skyia = Sun::GetLocalSetTime(FixTime(2024, 10, 2));

	TEST_HTT_1(skyia - (TOSEFET_KNISAT_KODESH_SECONDS + 1), Kodesh::ErevChag,							EREV_KODESH);
	TEST_HTT_1(skyia - TOSEFET_KNISAT_KODESH_SECONDS,		Kodesh::AchareiHadlaka,						HADLAKA);
	TEST_HTT_1(skyia - 1,									Kodesh::AchareiHadlaka,						HADLAKA);
	TEST_HTT_1(skyia,										Kodesh::RoshHaShanaA,						KODESH);
	TEST_HTT_1(skyia + 1,									Kodesh::RoshHaShanaA,						KODESH);

	skyia = Sun::GetLocalSetTime(skyia + SECONDS_PER_DAY);

	TEST_HTT_1(skyia - 1,									Kodesh::RoshHaShanaA,						KODESH);
	TEST_HTT_1(skyia,										Kodesh::RoshHaShanaA,						KODESH);
	TEST_HTT_1(skyia + TOSEFET_TZET_KODESH_SECONDS - 1,		Kodesh::RoshHaShanaA,						KODESH);
	TEST_HTT_1(skyia + TOSEFET_TZET_KODESH_SECONDS,			Kodesh::RoshHaShanaB,						KODESH);
	
	skyia = Sun::GetLocalSetTime(skyia + SECONDS_PER_DAY);

	TEST_HTT_1(skyia - 1,									Kodesh::RoshHaShanaB,						KODESH);
	TEST_HTT_1(skyia,										Kodesh::RoshHaShanaB | Kodesh::Shabat,		KODESH);
	TEST_HTT_1(skyia + TOSEFET_TZET_KODESH_SECONDS - 1,		Kodesh::RoshHaShanaB | Kodesh::Shabat,		KODESH);
	TEST_HTT_1(skyia + TOSEFET_TZET_KODESH_SECONDS,			Kodesh::Shabat,								KODESH);

	skyia = Sun::GetLocalSetTime(skyia + SECONDS_PER_DAY);

	TEST_HTT_1(skyia - 1,									Kodesh::Shabat,								KODESH);
	TEST_HTT_1(skyia,										Kodesh::Shabat | Kodesh::AchareiShkyia,		KODESH);
	TEST_HTT_1(skyia + TOSEFET_TZET_KODESH_SECONDS - 1,		Kodesh::Shabat | Kodesh::AchareiShkyia,		KODESH);
	TEST_HTT_1(skyia + TOSEFET_TZET_KODESH_SECONDS,			Kodesh::Chol,								CHOL);

	FixTime t;
	FixTime before = FixTime(2020, 4, 28);

	t = Kodesh::GetHadlaka(before);
	if (!t.IsZero())		_LOGGER << __LINE__ << ". ERROR: " << t.ToText() << NL;
	t = Kodesh::GetHadlaka(before, true);
	_LOGGER << __LINE__ << ". " << t.ToText() << NL;

	t = Kodesh::GetMotzeyKodesh(before);
	if (!t.IsZero())		_LOGGER << __LINE__ << ". ERROR: " << t.ToText() << NL;
	t = Kodesh::GetMotzeyKodesh(before, true);
	_LOGGER << __LINE__ << ". " << t.ToText() << NL;

	_LOGGER << "Done!" << NL;
}

// the loop function runs over and over again until power down or reset
void loop() {
  
}
