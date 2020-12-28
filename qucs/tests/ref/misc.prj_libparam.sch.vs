module main();
Vdc #(.U(1 V)) V1(net_140_220, net_140_280);
GND #() anonymous_gnd_hack_0(net_140_320);
R #(.R(50 Ohm), .Temp(26.85), .Tc1(0.0), .Tc2(0.0), .Tnom(26.85), .Symbol(US)) R1(net_340_220, net_340_280);
Lib$Ideal$Notch2 #(.Lib(Ideal), .Component(Notch2), .fc(1E3), .Q(1)) IB1(net_220_200, net_280_200);
//DC1
wire #() noname(net_140_280, net_140_300);
wire #() noname(net_140_300, net_140_320);
wire #() noname(net_140_300, net_340_300);
wire #() noname(net_340_280, net_340_300);
wire #() noname(net_140_200, net_140_220);
wire #() noname(net_140_200, net_220_200);
wire #() noname(net_340_200, net_340_220);
wire #() noname(net_280_200, net_340_200);
wire #() noname(net_340_200, net_340_200);
place #(.$xposition(140),.$yposition(200)) place_140_200(net_140_200);
place #(.$xposition(140),.$yposition(220)) place_140_220(net_140_220);
place #(.$xposition(140),.$yposition(280)) place_140_280(net_140_280);
place #(.$xposition(140),.$yposition(300)) place_140_300(net_140_300);
place #(.$xposition(140),.$yposition(320)) place_140_320(net_140_320);
place #(.$xposition(220),.$yposition(200)) place_220_200(net_220_200);
place #(.$xposition(280),.$yposition(200)) place_280_200(net_280_200);
place #(.$xposition(340),.$yposition(200)) place_340_200(net_340_200);
place #(.$xposition(340),.$yposition(220)) place_340_220(net_340_220);
place #(.$xposition(340),.$yposition(280)) place_340_280(net_340_280);
place #(.$xposition(340),.$yposition(300)) place_340_300(net_340_300);
endmodule // main

Sub #(.File()) Sub(net_0_0);
// skip sckt :SymbolSection:
// skip sckt :Diagrams:
// skip sckt :Paintings:
