module main();
GND #() anonymous_gnd_hack_0(net_120_330);
GND #() anonymous_gnd_hack_1(net_190_330);
GND #() anonymous_gnd_hack_2(net_280_330);
R #(.R(100k), .Temp(26.85), .Tc1(0.0), .Tc2(0.0), .Tnom(26.85), .Symbol(european)) R1(net_120_180, net_120_120);
R #(.R(10k), .Temp(26.85), .Tc1(0.0), .Tc2(0.0), .Tnom(26.85), .Symbol(european)) R2(net_120_280, net_120_220);
R #(.R(500), .Temp(26.85), .Tc1(0.0), .Tc2(0.0), .Tnom(26.85), .Symbol(european)) R3(net_190_330, net_190_270);
//DC1
GND #() anonymous_gnd_hack_3(net_20_330);
_BJT #(.Type(npn), .Is(1e-16), .Nf(1), .Nr(1), .Ikf(0), .Ikr(0), .Vaf(0), .Var(0), .Ise(0), .Ne(1.5), .Isc(0), .Nc(2), .Bf(100), .Br(1), .Rbm(0), .Irb(0), .Rc(0), .Re(0), .Rb(0), .Cje(0), .Vje(0.75), .Mje(0.33), .Cjc(0), .Vjc(0.75), .Mjc(0.33), .Xcjc(1.0), .Cjs(0), .Vjs(0.75), .Mjs(0), .Fc(0.5), .Tf(0.0), .Xtf(0.0), .Vtf(0.0), .Itf(0.0), .Tr(0.0), .Temp(26.85), .Kf(0.0), .Af(1.0), .Ffe(1.0), .Kb(0.0), .Ab(1.0), .Fb(1.0), .Ptf(0.0), .Xtb(0.0), .Xti(3.0), .Eg(1.11), .Tnom(26.85), .Area(1.0)) T1(net_160_200, net_190_170, net_190_230, net_190_170);
C #(.C(1.5n), .V(), .Symbol(neutral)) C1(net_190_140, net_190_80);
R #(.R(100k), .Temp(26.85), .Tc1(0.0), .Tc2(0.0), .Tnom(26.85), .Symbol(european)) R4(net_270_140, net_270_80);
L #(.L(80u), .I()) L1(net_350_140, net_350_80);
Vac #(.U(1 uV), .f(1 GHz), .Phase(0), .Theta(0)) V2(net_20_240, net_20_300);
C #(.C(100n), .V(), .Symbol(neutral)) C2(net_280_330, net_280_270);
GND #() anonymous_gnd_hack_4(net_420_230);
Vdc #(.U(12 V)) V1(net_420_170, net_420_230);
//AC1
Eqn #(.Gain(dB(Output.v/Input.v)), .Export(yes)) Eqn1();
wire #() noname(net_120_180, net_120_200);
wire #() noname(net_120_200, net_120_220);
wire #() noname(net_120_200, net_160_200);
wire #() noname(net_190_230, net_190_250);
wire #() noname(net_280_250, net_280_270);
wire #() noname(net_190_250, net_190_270);
wire #() noname(net_190_250, net_280_250);
wire #() noname(net_120_280, net_120_330);
wire #() noname(net_100_200, net_120_200);
wire #() noname(net_20_200, net_20_240);
wire #() noname(net_20_200, net_40_200);
wire #() noname(net_20_300, net_20_330);
wire #() noname(net_190_140, net_270_140);
wire #() noname(net_190_140, net_190_170);
wire #() noname(net_120_80, net_190_80);
wire #() noname(net_120_80, net_120_120);
wire #() noname(net_190_80, net_270_80);
wire #() noname(net_270_80, net_350_80);
wire #() noname(net_270_140, net_350_140);
wire #() noname(net_420_80, net_420_170);
wire #() noname(net_350_80, net_420_80);
wire #() noname(net_120_200, net_120_200);
place #(.$xposition(20),.$yposition(200)) place_20_200(net_20_200);
place #(.$xposition(20),.$yposition(240)) place_20_240(net_20_240);
place #(.$xposition(20),.$yposition(300)) place_20_300(net_20_300);
place #(.$xposition(20),.$yposition(330)) place_20_330(net_20_330);
place #(.$xposition(40),.$yposition(200)) place_40_200(net_40_200);
place #(.$xposition(100),.$yposition(200)) place_100_200(net_100_200);
place #(.$xposition(120),.$yposition(80)) place_120_80(net_120_80);
place #(.$xposition(120),.$yposition(120)) place_120_120(net_120_120);
place #(.$xposition(120),.$yposition(180)) place_120_180(net_120_180);
place #(.$xposition(120),.$yposition(200)) place_120_200(net_120_200);
place #(.$xposition(120),.$yposition(220)) place_120_220(net_120_220);
place #(.$xposition(120),.$yposition(280)) place_120_280(net_120_280);
place #(.$xposition(120),.$yposition(330)) place_120_330(net_120_330);
place #(.$xposition(160),.$yposition(200)) place_160_200(net_160_200);
place #(.$xposition(190),.$yposition(80)) place_190_80(net_190_80);
place #(.$xposition(190),.$yposition(140)) place_190_140(net_190_140);
place #(.$xposition(190),.$yposition(170)) place_190_170(net_190_170);
place #(.$xposition(190),.$yposition(230)) place_190_230(net_190_230);
place #(.$xposition(190),.$yposition(250)) place_190_250(net_190_250);
place #(.$xposition(190),.$yposition(270)) place_190_270(net_190_270);
place #(.$xposition(190),.$yposition(330)) place_190_330(net_190_330);
place #(.$xposition(270),.$yposition(80)) place_270_80(net_270_80);
place #(.$xposition(270),.$yposition(140)) place_270_140(net_270_140);
place #(.$xposition(280),.$yposition(250)) place_280_250(net_280_250);
place #(.$xposition(280),.$yposition(270)) place_280_270(net_280_270);
place #(.$xposition(280),.$yposition(330)) place_280_330(net_280_330);
place #(.$xposition(350),.$yposition(80)) place_350_80(net_350_80);
place #(.$xposition(350),.$yposition(140)) place_350_140(net_350_140);
place #(.$xposition(420),.$yposition(80)) place_420_80(net_420_80);
place #(.$xposition(420),.$yposition(170)) place_420_170(net_420_170);
place #(.$xposition(420),.$yposition(230)) place_420_230(net_420_230);
endmodule // main

Sub #(.File()) Sub(net_0_0);
// skip sckt :SymbolSection:
// skip sckt :Diagrams:
// skip sckt :Paintings:
