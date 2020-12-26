module main(net_260_180, net_260_360, net_460_280, net_460_140, net_340_390);
sTr #(.T1(0.5), .T2(0.5)) Tr1(net_410_140, net_350_180, net_350_240, net_410_280, net_410_220, net_410_200);
Port #(.Num(3), .Type(analog)) P3(net_460_280);
Port #(.Num(4), .Type(analog)) P4(net_460_140);
Port #(.Num(5), .Type(analog)) P5(net_340_390);
Port #(.Num(2), .Type(analog)) P2(net_260_360);
Port #(.Num(1), .Type(analog)) P1(net_260_180);
wire #() noname(net_410_220, net_430_220);
wire #() noname(net_410_200, net_430_200);
wire #() noname(net_430_200, net_430_220);
wire #() noname(net_410_280, net_460_280);
wire #() noname(net_410_140, net_460_140);
wire #() noname(net_340_240, net_350_240);
wire #() noname(net_340_240, net_340_390);
wire #() noname(net_430_220, net_430_360);
wire #() noname(net_260_360, net_430_360);
wire #() noname(net_260_180, net_350_180);
place #(.$xposition(260),.$yposition(180)) place_260_180(net_260_180);
place #(.$xposition(260),.$yposition(360)) place_260_360(net_260_360);
place #(.$xposition(340),.$yposition(240)) place_340_240(net_340_240);
place #(.$xposition(340),.$yposition(390)) place_340_390(net_340_390);
place #(.$xposition(350),.$yposition(180)) place_350_180(net_350_180);
place #(.$xposition(350),.$yposition(240)) place_350_240(net_350_240);
place #(.$xposition(410),.$yposition(140)) place_410_140(net_410_140);
place #(.$xposition(410),.$yposition(200)) place_410_200(net_410_200);
place #(.$xposition(410),.$yposition(220)) place_410_220(net_410_220);
place #(.$xposition(410),.$yposition(280)) place_410_280(net_410_280);
place #(.$xposition(430),.$yposition(200)) place_430_200(net_430_200);
place #(.$xposition(430),.$yposition(220)) place_430_220(net_430_220);
place #(.$xposition(430),.$yposition(360)) place_430_360(net_430_360);
place #(.$xposition(460),.$yposition(140)) place_460_140(net_460_140);
place #(.$xposition(460),.$yposition(280)) place_460_280(net_460_280);
endmodule // main

Sub #(.File()) Sub(net_0_0);
// skip sckt :SymbolSection:
// skip sckt :Diagrams:
// skip sckt :Paintings: