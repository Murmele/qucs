module main();
Lib$Z-Diodes$1N4732A #(.Lib(Z-Diodes), .Component(1N4732A)) D1(net_80_260, net_80_320);
_BJT #(.Type(npn), .Is(15.5f), .Nf(1), .Nr(1), .Ikf(54.7m), .Ikr(0.135), .Vaf(127), .Var(28), .Ise(3.9p), .Ne(2), .Isc(0), .Nc(2), .Bf(524), .Br(4), .Rbm(0), .Irb(0), .Rc(0.116), .Re(0.29), .Rb(1.16), .Cje(22.7p), .Vje(1.1), .Mje(0.5), .Cjc(7.34p), .Vjc(0.3), .Mjc(0.3), .Xcjc(1), .Cjs(0), .Vjs(0.75), .Mjs(0), .Fc(0.5), .Tf(698p), .Xtf(0), .Vtf(0), .Itf(0), .Tr(121n), .Temp(26.85), .Kf(0), .Af(1), .Ffe(1), .Kb(0), .Ab(1), .Fb(1), .Ptf(0), .Xtb(0.0), .Xti(3.0), .Eg(1.11), .Tnom(26.85), .Area(1.0)) T_2DC4617R1(net_230_290, net_200_260, net_200_320, net_200_260);
_BJT #(.Type(pnp), .Is(15.2f), .Nf(1), .Nr(1), .Ikf(54.7m), .Ikr(0.135), .Vaf(127), .Var(24), .Ise(3.88p), .Ne(2), .Isc(0), .Nc(2), .Bf(523), .Br(4), .Rbm(0), .Irb(0), .Rc(0.284), .Re(0.710), .Rb(2.84), .Cje(42.6p), .Vje(1.1), .Mje(0.5), .Cjc(13.7p), .Vjc(0.3), .Mjc(0.3), .Xcjc(1), .Cjs(0), .Vjs(0.75), .Mjs(0), .Fc(0.5), .Tf(453p), .Xtf(0), .Vtf(0), .Itf(0), .Tr(96.4n), .Temp(26.85), .Kf(0), .Af(1), .Ffe(1), .Kb(0), .Ab(1), .Fb(1), .Ptf(0), .Xtb(0.0), .Xti(3.0), .Eg(1.11), .Tnom(26.85), .Area(1.0)) T_2DA1774R1(net_230_230, net_260_260, net_260_200, net_260_260);
GND #() anonymous_gnd_hack_0(net_200_320);
GND #() anonymous_gnd_hack_1(net_80_320);
C #(.C(1n), .V(), .Symbol(neutral)) C1(net_320_300, net_320_240);
GND #() anonymous_gnd_hack_2(net_320_320);
Vdc #(.U(10)) V1(net_420_100, net_420_160);
GND #() anonymous_gnd_hack_3(net_420_160);
R #(.R(400), .Temp(26.85), .Tc1(0.0), .Tc2(0.0), .Tnom(26.85), .Symbol(european)) R1(net_320_160, net_320_100);
R #(.R(12), .Temp(26.85), .Tc1(0.0), .Tc2(0.0), .Tnom(26.85), .Symbol(european)) R2(net_80_160, net_80_100);
//TR1
wire #() noname(net_80_230, net_80_260);
wire #() noname(net_80_230, net_200_230);
wire #() noname(net_200_230, net_230_230);
wire #() noname(net_200_230, net_200_260);
wire #() noname(net_260_260, net_260_290);
wire #() noname(net_230_290, net_260_290);
wire #() noname(net_320_300, net_320_320);
wire #() noname(net_80_80, net_80_100);
wire #() noname(net_420_80, net_420_100);
wire #() noname(net_320_160, net_320_180);
wire #() noname(net_260_180, net_320_180);
wire #() noname(net_80_80, net_320_80);
wire #() noname(net_320_80, net_420_80);
wire #() noname(net_320_80, net_320_100);
wire #() noname(net_260_180, net_260_200);
wire #() noname(net_320_180, net_320_240);
wire #() noname(net_80_160, net_80_230);
wire #() noname(net_320_180, net_320_180);
place #(.$xposition(80),.$yposition(80)) place_80_80(net_80_80);
place #(.$xposition(80),.$yposition(100)) place_80_100(net_80_100);
place #(.$xposition(80),.$yposition(160)) place_80_160(net_80_160);
place #(.$xposition(80),.$yposition(230)) place_80_230(net_80_230);
place #(.$xposition(80),.$yposition(260)) place_80_260(net_80_260);
place #(.$xposition(80),.$yposition(320)) place_80_320(net_80_320);
place #(.$xposition(200),.$yposition(230)) place_200_230(net_200_230);
place #(.$xposition(200),.$yposition(260)) place_200_260(net_200_260);
place #(.$xposition(200),.$yposition(320)) place_200_320(net_200_320);
place #(.$xposition(230),.$yposition(230)) place_230_230(net_230_230);
place #(.$xposition(230),.$yposition(290)) place_230_290(net_230_290);
place #(.$xposition(260),.$yposition(180)) place_260_180(net_260_180);
place #(.$xposition(260),.$yposition(200)) place_260_200(net_260_200);
place #(.$xposition(260),.$yposition(260)) place_260_260(net_260_260);
place #(.$xposition(260),.$yposition(290)) place_260_290(net_260_290);
place #(.$xposition(320),.$yposition(80)) place_320_80(net_320_80);
place #(.$xposition(320),.$yposition(100)) place_320_100(net_320_100);
place #(.$xposition(320),.$yposition(160)) place_320_160(net_320_160);
place #(.$xposition(320),.$yposition(180)) place_320_180(net_320_180);
place #(.$xposition(320),.$yposition(240)) place_320_240(net_320_240);
place #(.$xposition(320),.$yposition(300)) place_320_300(net_320_300);
place #(.$xposition(320),.$yposition(320)) place_320_320(net_320_320);
place #(.$xposition(420),.$yposition(80)) place_420_80(net_420_80);
place #(.$xposition(420),.$yposition(100)) place_420_100(net_420_100);
place #(.$xposition(420),.$yposition(160)) place_420_160(net_420_160);
endmodule // main

Sub #(.File()) Sub(net_0_0);
// skip sckt :SymbolSection:
// skip sckt :Diagrams:
// skip sckt :Paintings:
