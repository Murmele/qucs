module main();
Vac #(.U(1 V), .f(1 GHz), .Phase(0), .Theta(0)) V1(net_120_120, net_120_180);
GND #() anonymous_gnd_hack_0(net_320_240);
GND #() anonymous_gnd_hack_1(net_120_240);
R #(.R(R_par), .Temp(26.85), .Tc1(0.0), .Tc2(0.0), .Tnom(26.85), .Symbol(european)) R1(net_320_120, net_320_60);
C #(.C(2 pF), .V(), .Symbol(neutral)) C1(net_320_240, net_320_180);
L #(.L(13 nH), .I()) L1(net_320_180, net_320_120);
//SW1
//AC1
wire #() noname(net_120_180, net_120_240);
wire #() noname(net_120_60, net_320_60);
wire #() noname(net_120_60, net_120_120);
wire #() noname(net_320_180, net_320_180);
place #(.$xposition(120),.$yposition(60)) place_120_60(net_120_60);
place #(.$xposition(120),.$yposition(120)) place_120_120(net_120_120);
place #(.$xposition(120),.$yposition(180)) place_120_180(net_120_180);
place #(.$xposition(120),.$yposition(240)) place_120_240(net_120_240);
place #(.$xposition(320),.$yposition(60)) place_320_60(net_320_60);
place #(.$xposition(320),.$yposition(120)) place_320_120(net_320_120);
place #(.$xposition(320),.$yposition(180)) place_320_180(net_320_180);
place #(.$xposition(320),.$yposition(240)) place_320_240(net_320_240);
endmodule // main

Sub #(.File()) Sub(net_0_0);
// skip sckt :SymbolSection:
// skip sckt :Diagrams:
// skip sckt :Paintings:
