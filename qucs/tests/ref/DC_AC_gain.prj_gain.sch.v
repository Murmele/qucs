module main();
Vac #(.U(1 uV), .f(2 kHz), .Phase(0), .Theta(0)) V4(Input, gnd);
GND #() *(gnd);
R #(.R(50k), .Temp(26.85), .Tc1(0.0), .Tc2(0.0), .Tnom(26.85), .Symbol(european)) R2(Output, _net12);
GND #() *(gnd);
R #(.R(100k), .Temp(26.85), .Tc1(0.0), .Tc2(0.0), .Tnom(26.85), .Symbol(european)) R1(gnd, Output);
GND #() *(gnd);
GND #() *(gnd);
Vdc #(.U(2.5 V)) V3(_net7, gnd);
GND #() *(gnd);
Vdc #(.U(-2.5 V)) V6(_net15, gnd);
//AC1
//DC1
Eqn #(.Gain(dB(Output.v/Input.v)), .Phase(phase(Output.v/Input.v)), .Export(yes)) Eqn1();
Sub$singleOPV #(.File(singleOPV.sch)) OPV1(_net12, Input, _net7, _net15, Output);
R #(.R(1.7k), .Temp(26.85), .Tc1(0.0), .Tc2(0.0), .Tnom(26.85), .Symbol(european)) R3(_net12, gnd);
endmodule // main

Sub #(.File()) Sub(_net0);
