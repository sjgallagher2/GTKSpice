/* GTKSpice is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * GTKSpice is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with GTKSpice.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SPICE_ELEMENT_H
#define SPICE_ELEMENT_H

#include <gtkmm.h>


/* SPICE Elements
 *  Resistor
 *  Semiconductor Resistor
 * 
 *  Capacitor
 *  Semiconductor Capacitor
 * 
 *  Inductor
 *  Coupled Inductors
 * 
 *  Switch
 *  Switch Model
 * 
 *  Independent Sources
 *      Pulse
 *      Sinusoidal
 *      Exponential
 *      Piece-Wise Linear (PWL)
 *      Single-Frequency FM (SFFM)
 *  Linear Dependent Sources
 *      Linear Voltage-Controlled Current Source (VCCS)
 *      Linear Voltage-Controlled Voltage Source (VCVS)
 *      Linear Current-Controlled Current Source (CCCS)
 *      Linear Current-Controlled Voltage Source (CCVS)
 *  Non-linear Dependent Sources
 * 
 *  Transmission Lines
 *      Lossless Transmission Line
 *      Lossy Transmission Line
 *      Uniform Distributed RC Line (Lossy)
 *  
 *  Transistors and Diodes
 *      Junction Diodes
 *      Diode Model
 *      Bipolar Junction Transistor (BJT)
 *      Junction Field-Effect Transistor (JFET)
 *      MOSFET
 *      MESFET
 * 
 *  Subcircuit
 *  
 * ******* NGSPICE ELEMENTS **********
 * 
 *  Behavioral resistor
 *  Behavioral capacitaor
 *  Behavior inductor
 * 
 *  Indepdent sources
 *      Amplitude modulated source (AM)
 *      Transient noise source
 *      Random voltage source
 *      External voltage or current input
 *      Arbitrary phase sources
 * 
 *  KSPICE Lossy Transmission Lines
 *      Single Lossy Transmission Line (TXL)
 *      Coupled Multiconductor Line)
 */

/* @brief Container for SPICE string data fields */
struct SpiceElement
{
    typedef Glib::ustring Datafield;
    Datafield name; // Reference designator, following prefix (e.g. R143 is prefix R + name 123)
    Datafield nodes;

    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret  = "";
        ret = prefix+name+" "+nodes;
        return ret.uppercase();
    }
protected:
    Datafield prefix; // Element letter
};

/* @form RXXXXXXX N1 N2 VALUE */
struct SpicePassive : public SpiceElement
{
    Datafield value; // Resistance, Capacitance, Inductance
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret  = "";
        ret = prefix+name+" "+nodes+" "+value;
        return ret.uppercase();
    }
};


/* @form RXXXXXXX N1 N2 VALUE */
struct SpiceResistor : public SpicePassive
{
    SpiceResistor() {prefix = "R";}
};

struct SpiceReactive : public SpicePassive
{
    Glib::ustring ic; // Initial condition in V or A
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = prefix+name+" "+nodes+" "+value;
        if(!ic.empty())
            ret.append(" "+ic);
        return ret.uppercase();
    }
};
/* @form CXXXXXXX N+ N- VALUE <IC=INCOND> */
struct SpiceCapacitor : public SpiceReactive
{
    SpiceCapacitor() {prefix = "C";}
};
/* @form LXXXXXXX N+ N- VALUE <IC=INCOND> */
struct SpiceInductor : public SpiceReactive
{
    SpiceInductor() {prefix = "L";}
};

/* @form KXXXXXXX LYYYYYYYY LZZZZZZZ VALUE  */
struct SpiceCoupledInductors : public SpiceElement
{
    SpiceCoupledInductors() {prefix="K";}
    Datafield L1,L2; // Names of coupled inductors
    Datafield value; // Coefficient of coupling, 0 < K < 1
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret  = "";
        ret = prefix+name+" "+L1+" "+L2+" "+value;
        return ret.uppercase();
    }
};

/* @form RXXXXXXX N1 N2 <VALUE> <MNAME> <L=LENGTH> <W=WIDTH> <TEMP=T>  */
struct SpiceSemiconductorResistor : public SpiceResistor
{
    SpiceSemiconductorResistor() {prefix="R";}
    Datafield modelname;
    Datafield length;
    Datafield width;
    Datafield temp;
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret  = "";
        ret.append(prefix+name+" "+nodes);
        if(!value.empty())
            ret.append(" "+value);
        if(!modelname.empty())
            ret.append(" "+modelname);
        if(!length.empty())
            ret.append(" L="+length);
        if(!width.empty())
            ret.append(" W="+width);
        if(!temp.empty())
            ret.append(" TEMP="+temp);
        return ret.uppercase();
    }
};
/* @form CXXXXXXX N1 N2 <VALUE> <MNAME> <L=LENGTH> <W=WIDTH> <IC=VAL>  */
struct SpiceSemiconductorCapacitor : public SpiceCapacitor
{
    SpiceSemiconductorCapacitor() {prefix="C";}
    Datafield modelname;
    Datafield length;
    Datafield width;
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret  = "";
        ret = prefix+name+" "+nodes;
        if(!value.empty())
            ret.append(" "+value);
        if(!modelname.empty())
            ret.append(" "+modelname);
        if(!length.empty())
            ret.append(" L="+length);
        if(!width.empty())
            ret.append(" W="+width);
        if(!ic.empty())
            ret.append(" IC="+ic);
        return ret.uppercase();
    }
};

struct SpiceSwitch : public SpiceElement
{
    Datafield modelname;
    Datafield value; // <ON><OFF>
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret  = "";
        ret = prefix+name+" "+nodes+" "+modelname;
        if(!value.empty())
            ret.append(" "+value);
        return ret.uppercase();
    }
};

/* @form SXXXXXXX N+ N- NC+ NC- MODEL <ON><OFF>  */
struct SpiceVoltageControlledSwitch : public SpiceSwitch
{
    SpiceVoltageControlledSwitch() {prefix="S";}
    Datafield modelname;
    Datafield value;

    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret  = "";
        ret = prefix+name+" "+nodes;
        if(!modelname.empty())
            ret.append(" "+modelname);
        if(!value.empty())
            ret.append(" "+value);
        return ret.uppercase();
    }
};

/* @form WXXXXXXX N+ N- VNAM MODEL <ON><OFF>  */
struct SpiceCurrentControlledSwitch : public SpiceSwitch
{
    SpiceCurrentControlledSwitch() {prefix="W";}
    Datafield vnam; // Name of voltage supplying current
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret  = "";
        ret = prefix+name+" "+nodes;
        if(!value.empty())
            ret.append(value);
        return ret.uppercase();
    }
};


struct SpiceIndependentSource : public SpiceElement
{
    Datafield dc;
    Datafield dc_value;
    
    Datafield ac;
    Datafield ac_mag;
    Datafield ac_phase;

    Datafield distof1;
    Datafield distof1_f1mag;
    Datafield distof1_f1phase;

    Datafield distof2;
    Datafield distof2_f2mag;
    Datafield distof2_f2phase;

    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret  = "";
        ret = prefix+name+" "+nodes;
        if(!dc_value.empty())
        {
            if(!dc.empty())
                ret.append(" "+dc);
            ret.append(" "+dc_value);
        }
        if(!ac.empty())
        {
            ret.append(" "+ac);
            if(!ac_mag.empty())
            {
                ret.append(" "+ac_mag);
                if(!ac_phase.empty())
                    ret.append(" "+ac_phase);
            }
        }
        if(!distof1.empty())
        {
            ret.append(" "+distof1);
            if(!distof1_f1mag.empty())
            {
                ret.append(" "+distof1_f1mag);
                if(!distof1_f1phase.empty())
                    ret.append(" "+distof1_f1phase);
            }
        }
        if(!distof2.empty())
        {
            ret.append(" "+distof2);
            if(!distof2_f2mag.empty())
            {
                ret.append(" "+distof2_f2mag);
                if(!distof2_f2phase.empty())
                    ret.append(" "+distof2_f2phase);
            }
        }
        return ret.uppercase();
    }
};
/* @form VXXXXXXX N+ N- <<DC> DC/TRAN VALUE> <AC <ACMAG <ACPHASE>>> <DISTOF1 <F1MAG <F1PHASE>>> <DISTOF2 <F2MAG <F2PHASE>>>  */
struct SpiceIndependentVoltageSource : SpiceIndependentSource
{
    SpiceIndependentVoltageSource() {prefix="V";}
};
/* @form IYYYYYYY N+ N- <<DC> DC/TRAN VALUE> <AC <ACMAG <ACPHASE>>> <DISTOF1 <F1MAG <F1PHASE>>> <DISTOF2 <F2MAG <F2PHASE>>>  */
struct SpiceIndependentCurrentSource : SpiceIndependentSource
{
    SpiceIndependentCurrentSource() {prefix="I";}
};

// These are time-dependent values for transient analysis
/* @form PULSE(V1 V2 TD TR TF PW PER)    */
struct SpicePulseSource
{
    typedef Glib::ustring Datafield;

    Datafield v1;   // First voltage
    Datafield v2;   // Second voltage
    Datafield td;   // Delay time
    Datafield tr;   // Rise time
    Datafield tf;   // Fall time
    Datafield pw;   // Pulse width
    Datafield per;  // Period
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret  = "SIN(";
        ret = ret + v1+" "+v2+" "+td+" "+tr+" "+tf+" "+pw+" "+per;
        ret = ret + ")";
        return ret.uppercase();
    }
};

/* @form SIN(VO VA FREQ TD THETA)    */
struct SpiceSinSource
{
    typedef Glib::ustring Datafield;

    Datafield vo;       // Offset voltage
    Datafield va;       // Voltage amplitude
    Datafield freq;     // Frequency
    Datafield td;       // Delay time
    Datafield theta;    // 

    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret  = "SIN(";
        ret = ret + vo+" "+va+" "+freq+" "+td+" "+theta;
        ret = ret + ")";
        return ret.uppercase();
    }
};

/* @form EXP(V1 V2 TD1 TAU1 TD2 TAU2)    */
struct SpiceExpSource
{
    typedef Glib::ustring Datafield;

    Datafield v1;   // First voltage
    Datafield v2;   // Second voltage
    Datafield td1;  // Delay time 1
    Datafield tau1; // Time constant 1
    Datafield td2;  // Delay time 2
    Datafield tau2; // Time constant 2

    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret  = "EXP(";
        ret = ret + v1+" "+v2+" "+td1+" "+tau1+" "+td2+" "+tau2;
        ret = ret + ")";
        return ret.uppercase();
    }
};

/* @form PWL(T1 V1 <T2 V2 T3 V3 T4 V4...>)    */
struct PWLSource
{
    typedef Glib::ustring Datafield;

    Datafield parameters; // T1 V1 T2 V2 ...
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret  = "PWL(";
        ret = ret + parameters;
        ret = ret + ")";
        return ret.uppercase();
    }
};

/* @form SFFM(VO VA FC MDI FS)    */
struct SFFMSource
{
    typedef Glib::ustring Datafield;

    Datafield vo;   // Offset voltage
    Datafield va;   // Amplitude voltage
    Datafield fc;   // Center frequency
    Datafield mdi;  // Modulation index
    Datafield fs;   // Signal frequency

    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret  = "SFFM(";
        ret = ret + vo+" "+va+" "+fc+" "+mdi+" "+fs;
        ret = ret + ")";
        return ret.uppercase();
    }
};

struct SpiceLinearDependentSource : public SpiceElement
{
    Datafield value;
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret  = "";
        ret = prefix+name+" "+nodes+" "+value;
        return ret.uppercase();
    }
};

/* @form GXXXXXXX N+ N- NC+ NC- VALUE    */
struct SpiceLinearVCCS : public SpiceLinearDependentSource 
{
    SpiceLinearVCCS() {prefix = "G";}
};

/* @form EXXXXXXX N+ N- NC+ NC- VALUE    */
struct SpiceLinearVCVS : public SpiceLinearDependentSource 
{
    SpiceLinearVCVS() {prefix = "E";}
};

/* @form FXXXXXXX N+ N- VNAM VALUE    */
struct SpiceLinearCCCS : public SpiceLinearDependentSource 
{
    SpiceLinearCCCS() {prefix = "F";}
};

/* @form HXXXXXXX N+ N- VNAM VALUE    */
struct SpiceLinearCCVS : public SpiceLinearDependentSource 
{
    SpiceLinearCCVS() {prefix = "H";}
};

/* @form BXXXXXXX N+ N- <I=EXPR> <V=EXPR>    */
struct SpiceNonlinearDependentSource : public SpiceElement
{
    SpiceNonlinearDependentSource() {prefix="B";}
    Datafield Iexpr;
    Datafield Vexpr;
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret  = "";
        ret = prefix+name+" "+nodes;
        if(!Iexpr.empty())
            ret = ret + " I="+Iexpr;
        if(!Vexpr.empty())
            ret = ret + " V="+Vexpr;
        return ret.uppercase();
    }
};

/* @form TXXXXXXX N1 N2 N3 N4 Z0=VALUE <TD=VALUE> <F=FREQ <NL=NRMLEN>> <IC=V1, I1, V2, I2>  */
struct SpiceLosslessTL : public SpiceElement
{
    SpiceLosslessTL() {prefix="T";}
    Datafield Z0;
    Datafield td;
    Datafield f;
    Datafield nl;
    Datafield ic;
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret  = "";
        ret = prefix+name+" "+nodes+" "+Z0;
        if(!td.empty())
            ret = ret+" TD="+td;
        if(!td.empty())
            ret = ret+" F="+f;
        if(!td.empty())
            ret = ret+" NL="+nl;
        if(!td.empty())
            ret = ret+" IC="+ic;
        return ret.uppercase();
    }
};

/* @form OXXXXXXX N1 N2 N3 N4 MNAME   */
struct SpiceLossyTL : public SpiceElement
{
    SpiceLossyTL() {prefix="O";}
    Datafield modelname;
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret  = "";
        ret = prefix+name+" "+nodes+" "+modelname;
        return ret.uppercase();
    }
};

/* @form UXXXXXXX N1 N2 N3 MNAME L=LEN <N=LUMPS>   */
struct SpiceLossyUniformDistributedRCLine : public SpiceElement
{
    SpiceLossyUniformDistributedRCLine() {prefix="U";}
    Datafield modelname;
    Datafield len;
    Datafield lumps;
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret  = "";
        ret = prefix+name+" "+nodes+" "+modelname;
        if(!len.empty())
            ret.append(" L="+len);
        if(!lumps.empty())
            ret.append(" N="+lumps);
        return ret.uppercase();
    }
};

struct SpiceSemiconductor : public SpiceElement
{
    Datafield modelname;
    Datafield area;
    Datafield off;
    Datafield ic;
    Datafield temp;
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret  = "";
        ret = prefix+name+" "+nodes+" "+modelname;
        if(!area.empty())
            ret.append(" "+area);
        if(!off.empty())
            ret.append(" "+off);
        if(!ic.empty())
            ret.append(" IC="+ic);
        if(!temp.empty())
            ret.append(" TEMP="+temp);
        return ret.uppercase();
    }
};

/* @form DXXXXXXX N+ N- MNAME <AREA> <OFF> <IC=VD> <TEMP=T> */
struct SpiceJunctionDiode : public SpiceSemiconductor
{
    SpiceJunctionDiode() {prefix = "D";}
};

/* @form QXXXXXXX NC NB NE <NS> MNAME <AREA> <OFF> <IC=VBE, VCE> <TEMP=T> */
struct SpiceBJT : public SpiceSemiconductor
{
    SpiceBJT() {prefix = "Q";}
};

/* @form JXXXXXXX ND NG NS MNAME <AREA> <OFF> <IC=VDS, VGS> <TEMP=T> */
struct SpiceJFET : public SpiceSemiconductor
{
    SpiceJFET() {prefix = "J";}
};

/* @form MXXXXXXX ND NG NS NB MNAME <L=VAL> <W=VAL> <AD=VAL> <AS=VAL>
         + <PD=VAL> <PS=VAL> <NRD=VAL> <NRS=VAL> <OFF>
         + <IC=VDS, VGS, VBS> <TEMP=T> */
struct SpiceMOSFET : public SpiceElement
{
    SpiceMOSFET() {prefix="M";}
    Datafield modelname;
    Datafield L;
    Datafield W;
    Datafield ad;
    Datafield as;
    Datafield pd;
    Datafield ps;
    Datafield nrd;
    Datafield nrs;
    Datafield off;
    Datafield ic;
    Datafield temp;
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret  = "";
        ret = prefix+name+" "+nodes+" "+modelname;
        if(!L.empty())
            ret.append(" L="+L);
        if(!W.empty())
            ret.append(" W="+W);
        if(!ad.empty())
            ret.append(" AD="+ad);
        if(!as.empty())
            ret.append(" AS="+as);
        if(!pd.empty())
            ret.append(" PD="+pd);
        if(!ps.empty())
            ret.append(" PS="+ps);
        if(!nrd.empty())
            ret.append(" NRD="+nrd);
        if(!nrs.empty())
            ret.append(" NRS="+nrs);
        if(!off.empty())
            ret.append(" "+off);
        if(!ic.empty())
            ret.append(" IC="+ic);
        if(!temp.empty())
            ret.append(" TEMP="+temp);
        return ret.uppercase();
    }
};

/* @form ZXXXXXXX ND NG NS MNAME <AREA> <OFF> <IC=VDS, VGS> */
struct SpiceMESFET : public SpiceElement
{
    SpiceMESFET() {prefix="Z";}
    Datafield modelname;
    Datafield area;
    Datafield off;
    Datafield ic;
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = prefix+name+" "+nodes+" "+modelname;
        if(!area.empty())
            ret.append(" "+area);
        if(!off.empty())
            ret.append(" "+off);
        if(!ic.empty())
            ret.append(" IC="+ic);
    }
};

/* @form XYYYYYYY N1 <N2 N3 ...> SUBNAM  
 * @notes
 *  This is the instantiation of a subcircuit, not its definition (cf. SpiceSubcktDirective)
 */
struct SpiceSubcircuit : public SpiceElement
{
    SpiceSubcircuit() {prefix="X";}
    Datafield subname;
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = prefix+name+" "+nodes+" "+subname;
        return ret.uppercase();
    }
};

#endif /* SPICE_ELEMENT_H */
