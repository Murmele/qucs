module main();
GND #() anonymous_gnd_hack_0(net_800_220);
Pac #(.Num(2), .Z(50 Ohm), .P(0 W), .f(1 GHz), .Temp(-273.15)) P1(net_800_140, net_800_200);
GND #() anonymous_gnd_hack_1(net_310_220);
//DC1
Eqn #(.S21_dB(dB(S[2,1])), .S11_dB(dB(S[1,1])), .Export(yes)) Eqn2();
Eqn #(.Ql(200), .Qc(5000), .f0(14e6), .Export(yes)) Eqn1();
//SP1
//AC1
GND #() anonymous_gnd_hack_2(net_100_220);
Pac #(.Num(1), .Z(50 Ohm), .P(0 dBm), .f(1 GHz), .Temp(-273.15)) P2(net_100_120, net_100_180);
GND #() anonymous_gnd_hack_3(net_630_290);
Vdc #(.U(1.1 V)) V1(net_630_220, net_630_280);
wire #() noname(net_630_90, net_630_140);
wire #() noname(net_590_140, net_630_140);
wire #() noname(net_590_200, net_630_200);
wire #() noname(net_630_200, net_630_220);
wire #() noname(net_580_90, net_630_90);
wire #() noname(net_630_90, net_800_90);
wire #() noname(net_800_90, net_800_140);
wire #() noname(net_800_200, net_800_220);
wire #() noname(net_440_90, net_520_90);
wire #() noname(net_310_90, net_310_140);
wire #() noname(net_270_140, net_310_140);
wire #() noname(net_270_200, net_310_200);
wire #() noname(net_310_90, net_380_90);
wire #() noname(net_310_200, net_310_220);
wire #() noname(net_100_90, net_310_90);
wire #() noname(net_100_90, net_100_120);
wire #() noname(net_100_180, net_100_220);
wire #() noname(net_630_280, net_630_290);
wire #() noname(net_800_90, net_800_90);
place #(.$xposition(100),.$yposition(90)) place_100_90(net_100_90);
place #(.$xposition(100),.$yposition(120)) place_100_120(net_100_120);
place #(.$xposition(100),.$yposition(180)) place_100_180(net_100_180);
place #(.$xposition(100),.$yposition(220)) place_100_220(net_100_220);
place #(.$xposition(270),.$yposition(140)) place_270_140(net_270_140);
place #(.$xposition(270),.$yposition(200)) place_270_200(net_270_200);
place #(.$xposition(310),.$yposition(90)) place_310_90(net_310_90);
place #(.$xposition(310),.$yposition(140)) place_310_140(net_310_140);
place #(.$xposition(310),.$yposition(200)) place_310_200(net_310_200);
place #(.$xposition(310),.$yposition(220)) place_310_220(net_310_220);
place #(.$xposition(380),.$yposition(90)) place_380_90(net_380_90);
place #(.$xposition(440),.$yposition(90)) place_440_90(net_440_90);
place #(.$xposition(520),.$yposition(90)) place_520_90(net_520_90);
place #(.$xposition(580),.$yposition(90)) place_580_90(net_580_90);
place #(.$xposition(590),.$yposition(140)) place_590_140(net_590_140);
place #(.$xposition(590),.$yposition(200)) place_590_200(net_590_200);
place #(.$xposition(630),.$yposition(90)) place_630_90(net_630_90);
place #(.$xposition(630),.$yposition(140)) place_630_140(net_630_140);
place #(.$xposition(630),.$yposition(200)) place_630_200(net_630_200);
place #(.$xposition(630),.$yposition(220)) place_630_220(net_630_220);
place #(.$xposition(630),.$yposition(280)) place_630_280(net_630_280);
place #(.$xposition(630),.$yposition(290)) place_630_290(net_630_290);
place #(.$xposition(800),.$yposition(90)) place_800_90(net_800_90);
place #(.$xposition(800),.$yposition(140)) place_800_140(net_800_140);
place #(.$xposition(800),.$yposition(200)) place_800_200(net_800_200);
place #(.$xposition(800),.$yposition(220)) place_800_220(net_800_220);
endmodule // main

Sub #(.File()) Sub(net_0_0);
// skip sckt :SymbolSection:
// skip sckt :Diagrams:
// skip sckt :Paintings:
