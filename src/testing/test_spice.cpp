/* Test for SPICE model */

#include <iostream>
#include <spice/spice_element.h>
#include <spice/spice_deck.h>
#include <spice/spice_directive.h>
#include <spice/spice_model.h>

// TESTING SPICE_ELEMENT
void test_spice()
{
    /* A SUBCIRCUIT */
    SpiceSubcktDirective stn3nf06l_dir;
    stn3nf06l_dir.nodes = "1 2 3";
    stn3nf06l_dir.subname = "STN3NF06L";
    SpiceInductor lg;
    lg.name = "LG";
    lg.nodes = "2 4";
    lg.value = "7.5n";
    SpiceInductor ls;
    ls.name = "LG";
    ls.nodes = "12 3";
    ls.value = "7.5n";
    SpiceEndsDirective stn3nf06l_ends;
    stn3nf06l_ends.subname = "STN3NF06L";

    SpiceSubcircuitDeck stn3nf06l;
    stn3nf06l.add_line(stn3nf06l_dir.get_spice_line());
    stn3nf06l.add_line(lg.get_spice_line());
    stn3nf06l.add_line(ls.get_spice_line());
    stn3nf06l.add_line(stn3nf06l_ends.get_spice_line());
    /****************/

    // Models
    SpiceModelR resmodel1;
    resmodel1.TC1 = "0.1";
    resmodel1.RSH = "1m";
    SpiceModelBJT bjtmodel1;
    bjtmodel1.type="NPN";
    bjtmodel1.IS = "1e-15";
    
    SpiceModelDirective resmodel_dir;
    resmodel_dir.modelname="vishayres";
    resmodel_dir.model = resmodel1.get_spice_line();
    SpiceModelDirective bjtmodel_dir;
    bjtmodel_dir.modelname="2n3904";
    bjtmodel_dir.model = bjtmodel1.get_spice_line();
    SpiceModelDirective jfetmodel_dir;
    jfetmodel_dir.modelname = "2N5458";
    jfetmodel_dir.model = "NJF(Vto=-2.882 Beta=488.9u Lambda=3.167m Rd=1 Rs=1 Cgs=4.627p \n+ Cgd=4p Pb=.5 Is=181.3f Kf=4.284E-18 Af=1 Fc=.5)";

    // Passives
    SpiceElement x1;
    x1.name = "X1";
    x1.nodes="1 2 3";
    SpiceInductor L1;
    L1.name="L1";
    L1.value="10u";
    L1.nodes="1 2";
    SpiceInductor L2;
    L2.name="L2";
    L2.nodes="2 3";
    L2.value="1m";
    SpiceCoupledInductors K1;
    K1.name="K1";
    K1.L1="L1";
    K1.L2="L2";
    K1.value="0.955";
	SpiceSemiconductorResistor res1;
	res1.name="R1";
	res1.nodes="1 0";
	res1.value="5k";
	SpiceSemiconductorResistor res2;
	res2.name="R2";
	res2.nodes="1 0";
    res2.modelname="vishayres";
	SpiceSemiconductorResistor res3;
	res3.name="R3";
	res3.nodes="1 0";
	res3.value="5k";
    res3.length="90n";
    res3.width="1u";
    SpiceSemiconductorCapacitor cap1;
	cap1.name="C1";
	cap1.nodes="1 0";
	cap1.value="5n";
	SpiceSemiconductorCapacitor cap2;
	cap2.name="C2";
	cap2.nodes="1 0";
    cap2.modelname="Cvishay10u";
	SpiceSemiconductorCapacitor cap3;
	cap3.name="C3";
	cap3.nodes="1 0";
	cap3.value="5u";
    cap3.length="90n";
    cap3.width="1u";
    SpiceSemiconductorResistor res4;
    res4.name="R4";
    res4.modelname=resmodel_dir.modelname;
    res4.nodes="2 3";
    SpiceBJT bjt1;
    bjt1.name="Q1";
    bjt1.nodes="1 2 3";
    bjt1.modelname = bjtmodel_dir.modelname;
    SpiceJFET jfet1;
    jfet1.name="J1";
    jfet1.nodes="3 4 0";
    jfet1.modelname=jfetmodel_dir.modelname;
    SpiceSubcircuit sub1;
    sub1.name = "X2";
    sub1.nodes = "2 3 4";
    sub1.subname = "STN3NF06L";

    // Directives (except model directives)
    SpiceTitleDirective title;
    title.title="Spice test 2019";
    SpiceOptionDirective opts;
    opts.keepopinfo = "keepopinfo";
    opts.method = "TRAP";
    SpiceTranDirective transim;
    transim.tstop="1m";
    transim.tstep="400";
    SpiceEndDirective end;

    // Spice deck
    SpiceDeck deck;
    deck.add_line(title.get_spice_line()); // Title
    deck.add_line(stn3nf06l.get_spice_lines());
    deck.add_line(bjtmodel_dir.get_spice_line());
    deck.add_line(resmodel_dir.get_spice_line());
    deck.add_line(jfetmodel_dir.get_spice_line());
    deck.add_line(x1.get_spice_line());
    deck.add_line(L1.get_spice_line());
    deck.add_line(L2.get_spice_line());
    deck.add_line(K1.get_spice_line());
    deck.add_line(res1.get_spice_line());
    deck.add_line(res2.get_spice_line());
    deck.add_line(res3.get_spice_line());
    deck.add_line(res4.get_spice_line());
    deck.add_line(cap1.get_spice_line());
    deck.add_line(cap2.get_spice_line());
    deck.add_line(cap3.get_spice_line());
    deck.add_line(bjt1.get_spice_line());
    deck.add_line(jfet1.get_spice_line());
    deck.add_line(sub1.get_spice_line());
    deck.add_line(opts.get_spice_line());
    deck.add_line(transim.get_spice_line());
    deck.add_line(end.get_spice_line());

    //deck.print();
    std::cout << deck.get_spice_lines();
}
