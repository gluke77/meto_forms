#ifndef _HMI_INCLUDED
#define _HMI_INCLUDED

#define TOUCH2_DEVICE_ID		(1)
#define PC_FOR_TOUCH2_DEVICE_ID	(0)
#define TOUCH2_BAUDRATE			(CBR_115200)
#define TOUCH2_PARITY			(NOPARITY)
#define TOUCH2_BYTESIZE			(7)
#define TOUCH2_STOPBITS			(2)

#define TOUCH3_DEVICE_ID		(2)
#define PC_FOR_TOUCH3_DEVICE_ID	(0)
#define TOUCH3_BAUDRATE			(CBR_115200)
#define TOUCH3_PARITY			(NOPARITY)
#define TOUCH3_BYTESIZE			(7)
#define TOUCH3_STOPBITS			(2)

///////////////////////
// Polyethilen extruder
///////////////////////

// Invertor

#define	PEM_ON			(10)
#define	B_PEM_ON		(10+0)
#define	R_PEM_RS		(11)
#define	RB_PEM_RS		(11+0)
#define	R_PEM_SV		(12)
#define	PEM_PV			(13)
#define	PEM_C			(14)
#define	PEM_CHC			(15)
#define	PEM_CHW			(16)
#define	PEM_CLW			(17)
#define	PEM_CLC			(18)
#define	W_PEM_RS		(19)
#define	WB_PEM_RS		(19+0)
#define	W_PEM_SV		(20)
#define	PW_PEM_RS		(21)
#define	PW_PEM_SV		(22)

// Zone 1

#define	PEZ1_ON			(30)
#define	B_PEZ1_ON		(30+0)
#define	R_PEZ1_RS		(31)
#define	RB_PEZ1_RS		(31+0)
#define	R_PEZ1_SV		(32)
#define	PEZ1_PV			(33)
#define	PEZ1_PVHC		(34)
#define	PEZ1_PVHW		(35)
#define	PEZ1_PVLW		(36)
#define	PEZ1_PVLC		(37)
#define	W_PEZ1_RS		(38)
#define	WB_PEZ1_RS		(38+0)
#define	W_PEZ1_SV		(39)
#define	PW_PEZ1_RS		(40)
#define	PW_PEZ1_SV		(41)

// Zone 2

#define	PEZ2_ON			(50)
#define	B_PEZ2_ON		(50+0)
#define	R_PEZ2_RS		(51)
#define	RB_PEZ2_RS		(51+0)
#define	R_PEZ2_SV		(52)
#define	PEZ2_PV			(53)
#define	PEZ2_PVHC		(54)
#define	PEZ2_PVHW		(55)
#define	PEZ2_PVLW		(56)
#define	PEZ2_PVLC		(57)
#define	W_PEZ2_RS		(58)
#define	WB_PEZ2_RS		(58+0)
#define	W_PEZ2_SV		(59)
#define	PW_PEZ2_RS		(60)
#define	PW_PEZ2_SV		(61)

// Zone 3

#define	PEZ3_ON			(70)
#define	B_PEZ3_ON		(70+0)
#define	R_PEZ3_RS		(71)
#define	RB_PEZ3_RS		(71+0)
#define	R_PEZ3_SV		(72)
#define	PEZ3_PV			(73)
#define	PEZ3_PVHC		(74)
#define	PEZ3_PVHW		(75)
#define	PEZ3_PVLW		(76)
#define	PEZ3_PVLC		(77)
#define	W_PEZ3_RS		(78)
#define	WB_PEZ3_RS		(78+0)
#define	W_PEZ3_SV		(79)
#define	PW_PEZ3_RS		(80)
#define	PW_PEZ3_SV		(81)

// Zone 4

#define	PEZ4_ON			(90)
#define	B_PEZ4_ON		(90+0)
#define	R_PEZ4_RS		(91)
#define	RB_PEZ4_RS		(91+0)
#define	R_PEZ4_SV		(92)
#define	PEZ4_PV			(93)
#define	PEZ4_PVHC		(94)
#define	PEZ4_PVHW		(95)
#define	PEZ4_PVLW		(96)
#define	PEZ4_PVLC		(97)
#define	W_PEZ4_RS		(98)
#define	WB_PEZ4_RS		(98+0)
#define	W_PEZ4_SV		(99)
#define	PW_PEZ4_RS		(100)
#define	PW_PEZ4_SV		(101)

// Trace

#define	PET_ON			(110)
#define	B_PET_ON		(110+0)
#define	R_PET_RS		(111)
#define	RB_PET_RS		(111+0)
#define	R_PET_SV		(112)
#define	PET_PV			(113)
#define	PET_PVHC		(114)
#define	PET_PVHW		(115)
#define	PET_PVLW		(116)
#define	PET_PVLC		(117)
#define	W_PET_RS		(118)
#define	WB_PET_RS		(118+0)
#define	W_PET_SV		(119)
#define	PW_PET_RS		(120)
#define	PW_PET_SV		(121)

// Pressure

#define	PEP_ON			(130)
#define	B_PEP_ON		(130+0)
#define	PEP_PV			(131)
#define	PEP_PVHC		(132)
#define	PEP_PVHW		(133)
#define	PEP_PVLW		(134)
#define	PEP_PVLC		(135)

///////////////////////
// Clue extruder
///////////////////////

// Invertor

#define	KLM_ON			(210)
#define	B_KLM_ON		(210+0)
#define	R_KLM_RS		(211)
#define	RB_KLM_RS		(211+0)
#define	R_KLM_SV		(212)
#define	KLM_PV			(213)
#define	KLM_C			(214)
#define	KLM_CHC			(215)
#define	KLM_CHW			(216)
#define	KLM_CLW			(217)
#define	KLM_CLC			(218)
#define	W_KLM_RS		(219)
#define	WB_KLM_RS		(219+0)
#define	W_KLM_SV		(220)
#define	PW_KLM_RS		(221)
#define	PW_KLM_SV		(222)

// Zone 1

#define	KLZ1_ON			(230)
#define	B_KLZ1_ON		(230+0)
#define	R_KLZ1_RS		(231)
#define	RB_KLZ1_RS		(231+0)
#define	R_KLZ1_SV		(232)
#define	KLZ1_PV			(233)
#define	KLZ1_PVHC		(234)
#define	KLZ1_PVHW		(235)
#define	KLZ1_PVLW		(236)
#define	KLZ1_PVLC		(237)
#define	W_KLZ1_RS		(238)
#define	WB_KLZ1_RS		(238+0)
#define	W_KLZ1_SV		(239)
#define	PW_KLZ1_RS		(240)
#define	PW_KLZ1_SV		(241)

// Zone 2

#define	KLZ2_ON			(250)
#define	B_KLZ2_ON		(250+0)
#define	R_KLZ2_RS		(251)
#define	RB_KLZ2_RS		(251+0)
#define	R_KLZ2_SV		(252)
#define	KLZ2_PV			(253)
#define	KLZ2_PVHC		(254)
#define	KLZ2_PVHW		(255)
#define	KLZ2_PVLW		(256)
#define	KLZ2_PVLC		(257)
#define	W_KLZ2_RS		(258)
#define	WB_KLZ2_RS		(258+0)
#define	W_KLZ2_SV		(259)
#define	PW_KLZ2_RS		(260)
#define	PW_KLZ2_SV		(261)

// Zone 3

#define	KLZ3_ON			(270)
#define	B_KLZ3_ON		(270+0)
#define	R_KLZ3_RS		(271)
#define	RB_KLZ3_RS		(271+0)
#define	R_KLZ3_SV		(272)
#define	KLZ3_PV			(273)
#define	KLZ3_PVHC		(274)
#define	KLZ3_PVHW		(275)
#define	KLZ3_PVLW		(276)
#define	KLZ3_PVLC		(277)
#define	W_KLZ3_RS		(278)
#define	WB_KLZ3_RS		(278+0)
#define	W_KLZ3_SV		(279)
#define	PW_KLZ3_RS		(280)
#define	PW_KLZ3_SV		(281)

// Zone 4

#define	KLZ4_ON			(290)
#define	B_KLZ4_ON		(290+0)
#define	R_KLZ4_RS		(291)
#define	RB_KLZ4_RS		(291+0)
#define	R_KLZ4_SV		(292)
#define	KLZ4_PV			(293)
#define	KLZ4_PVHC		(294)
#define	KLZ4_PVHW		(295)
#define	KLZ4_PVLW		(296)
#define	KLZ4_PVLC		(297)
#define	W_KLZ4_RS		(298)
#define	WB_KLZ4_RS		(298+0)
#define	W_KLZ4_SV		(299)
#define	PW_KLZ4_RS		(300)
#define	PW_KLZ4_SV		(301)

// Trace 4

#define	KLT_ON			(310)
#define	B_KLT_ON		(310+0)
#define	R_KLT_RS		(311)
#define	RB_KLT_RS		(311+0)
#define	R_KLT_SV		(312)
#define	KLT_PV			(313)
#define	KLT_PVHC		(314)
#define	KLT_PVHW		(315)
#define	KLT_PVLW		(316)
#define	KLT_PVLC		(317)
#define	W_KLT_RS		(318)
#define	WB_KLT_RS		(318+0)
#define	W_KLT_SV		(319)
#define	PW_KLT_RS		(320)
#define	PW_KLT_SV		(321)

// Pressure

#define	KLP_ON			(330)
#define	B_KLP_ON		(330+0)
#define	KLP_PV			(331)
#define	KLP_PVHC		(332)
#define	KLP_PVHW		(333)
#define	KLP_PVLW		(334)
#define	KLP_PVLC		(335)

///////////////////////
// Marker extruder
///////////////////////

// Invertor

#define	MEM_ON			(410)
#define	B_MEM_ON		(410+0)
#define	R_MEM_RS		(411)
#define	RB_MEM_RS		(411+0)
#define	R_MEM_SV		(412)
#define	MEM_PV			(413)
#define	MEM_C			(414)
#define	MEM_CHC			(415)
#define	MEM_CHW			(416)
#define	MEM_CLW			(417)
#define	MEM_CLC			(418)
#define	W_MEM_RS		(419)
#define	WB_MEM_RS		(419+0)
#define	W_MEM_SV		(420)
#define	PW_MEM_RS		(421)
#define	PW_MEM_SV		(422)

// Zone 1

#define	MEZ1_ON			(430)
#define	B_MEZ1_ON		(430+0)
#define	R_MEZ1_RS		(431)
#define	RB_MEZ1_RS		(431+0)
#define	R_MEZ1_SV		(432)
#define	MEZ1_PV			(433)
#define	MEZ1_PVHC		(434)
#define	MEZ1_PVHW		(435)
#define	MEZ1_PVLW		(436)
#define	MEZ1_PVLC		(437)
#define	W_MEZ1_RS		(438)
#define	WB_MEZ1_RS		(438+0)
#define	W_MEZ1_SV		(439)
#define	PW_MEZ1_RS		(440)
#define	PW_MEZ1_SV		(441)

// Zone 2

#define	MEZ2_ON			(450)
#define	B_MEZ2_ON		(450+0)
#define	R_MEZ2_RS		(451)
#define	RB_MEZ2_RS		(451+0)
#define	R_MEZ2_SV		(452)
#define	MEZ2_PV			(453)
#define	MEZ2_PVHC		(454)
#define	MEZ2_PVHW		(455)
#define	MEZ2_PVLW		(456)
#define	MEZ2_PVLC		(457)
#define	W_MEZ2_RS		(458)
#define	WB_MEZ2_RS		(458+0)
#define	W_MEZ2_SV		(459)
#define	PW_MEZ2_RS		(460)
#define	PW_MEZ2_SV		(461)

// Zone 3

#define	MEZ3_ON			(470)
#define	B_MEZ3_ON		(470+0)
#define	R_MEZ3_RS		(471)
#define	RB_MEZ3_RS		(471+0)
#define	R_MEZ3_SV		(472)
#define	MEZ3_PV			(473)
#define	MEZ3_PVHC		(474)
#define	MEZ3_PVHW		(475)
#define	MEZ3_PVLW		(476)
#define	MEZ3_PVLC		(477)
#define	W_MEZ3_RS		(478)
#define	WB_MEZ3_RS		(478+0)
#define	W_MEZ3_SV		(479)
#define	PW_MEZ3_RS		(480)
#define	PW_MEZ3_SV		(481)

// Zone 4

#define	MEZ4_ON			(490)
#define	B_MEZ4_ON		(490+0)
#define	R_MEZ4_RS		(491)
#define	RB_MEZ4_RS		(491+0)
#define	R_MEZ4_SV		(492)
#define	MEZ4_PV			(493)
#define	MEZ4_PVHC		(494)
#define	MEZ4_PVHW		(495)
#define	MEZ4_PVLW		(496)
#define	MEZ4_PVLC		(497)
#define	W_MEZ4_RS		(498)
#define	WB_MEZ4_RS		(498+0)
#define	W_MEZ4_SV		(499)
#define	PW_MEZ4_RS		(500)
#define	PW_MEZ4_SV		(501)

// Trace

#define	MET_ON			(510)
#define	B_MET_ON		(510+0)
#define	R_MET_RS		(511)
#define	RB_MET_RS		(511+0)
#define	R_MET_SV		(512)
#define	MET_PV			(513)
#define	MET_PVHC		(514)
#define	MET_PVHW		(515)
#define	MET_PVLW		(516)
#define	MET_PVLC		(517)
#define	W_MET_RS		(518)
#define	WB_MET_RS		(518+0)
#define	W_MET_SV		(519)
#define	PW_MET_RS		(520)
#define	PW_MET_SV		(521)

// Pressure

#define	MEP_ON			(530)
#define	B_MEP_ON		(530+0)
#define	MEP_PV			(531)
#define	MEP_PVHC		(532)
#define	MEP_PVHW		(533)
#define	MEP_PVLW		(534)
#define	MEP_PVLC		(535)

#endif // _HMI_INCLUDED
