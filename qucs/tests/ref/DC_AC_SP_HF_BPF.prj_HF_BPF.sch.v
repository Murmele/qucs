module main();
GND #() *(gnd);
Pac #(.Num(2), .Z(50 Ohm), .P(0 W), .f(1 GHz), .Temp(-273.15)) P1(out, gnd);
GND #() *(gnd);
//DC1
Eqn #(.S21_dB(dB(S[2,1])), .S11_dB(dB(S[1,1])), .Export(yes)) Eqn2();
Eqn #(.Ql(200), .Qc(5000), .f0(14e6), .Export(yes)) Eqn1();
//SP1
//AC1
GND #() *(gnd);
Pac #(.Num(1), .Z(50 Ohm), .P(0 dBm), .f(1 GHz), .Temp(-273.15)) P2(_net12, gnd);
GND #() *(gnd);
Vdc #(.U(1.1 V)) V1(_net10, gnd);
endmodule // main

Sub #(.File()) Sub(_net0);
