module main();
GND #() anonymous_gnd_hack_0(net_260_280);
C #(.C(1 uF), .V(), .Symbol(neutral)) C1(net_180_150, net_240_150);
Pac #(.Num(1), .Z(50 Ohm), .P(0 dBm), .f(1 GHz), .Temp(26.85)) P1(net_70_130, net_70_190);
GND #() anonymous_gnd_hack_1(net_70_190);
OpAmp #(.G(1e6), .Umax(15 V)) OP1(net_260_150, net_260_110, net_330_130);
R #(.R(10 Ohm), .Temp(26.85), .Tc1(0.0), .Tc2(0.0), .Tnom(26.85), .Symbol(european)) R1(net_180_110, net_240_110);
R #(.R(500k), .Temp(26.85), .Tc1(0.0), .Tc2(0.0), .Tnom(26.85), .Symbol(european)) R2(net_260_280, net_260_220);
GND #() anonymous_gnd_hack_2(net_530_160);
GND #() anonymous_gnd_hack_3(net_420_160);
Pac #(.Num(2), .Z(50 Ohm), .P(0 dBm), .f(1 GHz), .Temp(26.85)) P2(net_420_100, net_420_160);
R #(.R(500k), .Temp(26.85), .Tc1(0.0), .Tc2(0.0), .Tnom(26.85), .Symbol(european)) R4(net_530_160, net_530_100);
R #(.R(10 Ohm), .Temp(26.85), .Tc1(0.0), .Tc2(0.0), .Tnom(26.85), .Symbol(european)) R3(net_640_160, net_640_100);
GND #() anonymous_gnd_hack_4(net_640_250);
L #(.L(5 H), .I()) L1(net_640_250, net_640_190);
//SP1
//DC1
wire #() noname(net_240_110, net_260_110);
wire #() noname(net_240_150, net_260_150);
wire #() noname(net_260_70, net_260_110);
wire #() noname(net_180_110, net_180_150);
wire #() noname(net_260_150, net_260_220);
wire #() noname(net_70_110, net_70_130);
wire #() noname(net_70_110, net_180_110);
wire #() noname(net_330_70, net_330_130);
wire #() noname(net_260_70, net_330_70);
wire #() noname(net_420_80, net_420_100);
wire #() noname(net_640_80, net_640_100);
wire #() noname(net_420_80, net_530_80);
wire #() noname(net_530_80, net_640_80);
wire #() noname(net_530_80, net_530_100);
wire #() noname(net_640_160, net_640_190);
place #(.$xposition(70),.$yposition(110)) place_70_110(net_70_110);
place #(.$xposition(70),.$yposition(130)) place_70_130(net_70_130);
place #(.$xposition(70),.$yposition(190)) place_70_190(net_70_190);
place #(.$xposition(180),.$yposition(110)) place_180_110(net_180_110);
place #(.$xposition(180),.$yposition(150)) place_180_150(net_180_150);
place #(.$xposition(240),.$yposition(110)) place_240_110(net_240_110);
place #(.$xposition(240),.$yposition(150)) place_240_150(net_240_150);
place #(.$xposition(260),.$yposition(70)) place_260_70(net_260_70);
place #(.$xposition(260),.$yposition(110)) place_260_110(net_260_110);
place #(.$xposition(260),.$yposition(150)) place_260_150(net_260_150);
place #(.$xposition(260),.$yposition(220)) place_260_220(net_260_220);
place #(.$xposition(260),.$yposition(280)) place_260_280(net_260_280);
place #(.$xposition(330),.$yposition(70)) place_330_70(net_330_70);
place #(.$xposition(330),.$yposition(130)) place_330_130(net_330_130);
place #(.$xposition(420),.$yposition(80)) place_420_80(net_420_80);
place #(.$xposition(420),.$yposition(100)) place_420_100(net_420_100);
place #(.$xposition(420),.$yposition(160)) place_420_160(net_420_160);
place #(.$xposition(530),.$yposition(80)) place_530_80(net_530_80);
place #(.$xposition(530),.$yposition(100)) place_530_100(net_530_100);
place #(.$xposition(530),.$yposition(160)) place_530_160(net_530_160);
place #(.$xposition(640),.$yposition(80)) place_640_80(net_640_80);
place #(.$xposition(640),.$yposition(100)) place_640_100(net_640_100);
place #(.$xposition(640),.$yposition(160)) place_640_160(net_640_160);
place #(.$xposition(640),.$yposition(190)) place_640_190(net_640_190);
place #(.$xposition(640),.$yposition(250)) place_640_250(net_640_250);
endmodule // main

Sub #(.File()) Sub(net_0_0);
// skip sckt :SymbolSection:
// skip sckt :Diagrams:
// skip sckt :Paintings:
