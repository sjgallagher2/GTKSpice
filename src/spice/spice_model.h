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

#ifndef SPICE_MODEL_H
#define SPICE_MODEL_H

#include <gtkmm.h>

/* This explicitly lists the parameters of components as Glib::ustring types.
 * For use with a SpiceModelDirective (which formats the .MODEL MNAME datafields)
 */

/* @form: [.MODEL MNAME] TYPE(PNAME1=PVAL1 PNAME2=PVAL2 ... )   */

struct SpiceModel
{
    typedef Glib::ustring Param;
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret = type+"()";
        return ret.uppercase();
    }
protected:
    Param type = "";
};
struct SpiceModelR : public SpiceModel
{
    Param TC1;      // First order temperature coefficient (Ohm/C)
    Param TC2;      // Second order temperature coefficient (Ohm/C^2)
    Param RSH;      // Sheet resistance (Ohm/m^2)
    Param DEFW;     // Default width (meters)
    Param NARROW;   // Narrowing due to side etching (meters)
    Param TNOM;     // Parameter measurement temperature (deg C)
    // Nominal resistance is:
    // R = RSH (L-NARROW)/(W-NARROW)
    // Resistance is adjusted for temperature by:
    // R(T) = R(T0) [ 1 + TC1 (T − T0) + TC2 (T−T0)^2 ]
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret = type+"(";
        if(!TC1.empty()) ret.append("TC1="+TC1);
        if(!TC2.empty()) ret.append(" TC2="+TC2);
        if(!RSH.empty()) ret.append(" RSH="+RSH);
        if(!DEFW.empty()) ret.append(" DEFW="+DEFW);
        if(!NARROW.empty()) ret.append(" NARROW="+NARROW);
        if(!TNOM.empty()) ret.append(" TNOM="+TNOM);
        ret.append(")");
        return ret.uppercase();
    }
protected:
    Param type = "R";
};

struct SpiceModelC : public SpiceModel
{
    Param CJ;       // Junction bottom capacitance (F/m^2)
    Param CJSW;     // Junction sidewall capacitance (F/m)
    Param DEFW;     // Default device width (m)
    Param NARROW;   // Narrowing due to side etching (m)
    // Capacitance is:
    // CAP = CJ (LENGTH − NARROW) (WIDTH − NARROW) + 2 CJSW (LENGTH + WIDTH − 2 NARROW)
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret = type+"(";
        if(!CJ.empty()) ret.append("CJ="+CJ);
        if(!CJSW.empty()) ret.append(" CJSW="+CJSW);
        if(!DEFW.empty()) ret.append(" DEFW="+DEFW);
        if(!NARROW.empty()) ret.append(" NARROW="+NARROW);
        ret.append(")");
        return ret.uppercase();
    }
protected:
    Param type = "C";
};

struct SpiceModelSW : public SpiceModel
{
    Param VT;   // Threshold voltage (V)
    Param VH;   // Hysteresis voltage (V)
    Param RON;  // On resistance (Ohm)
    Param ROFF; // Off resistance (Ohm)
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret = type+"(";
        if(!VT.empty()) ret.append("VT="+VT);
        if(!VH.empty()) ret.append(" VH="+VH);
        if(!RON.empty()) ret.append(" RON="+RON);
        if(!ROFF.empty()) ret.append(" ROFF="+ROFF);
        ret.append(")");
        return ret.uppercase();
    }
protected:
    Param type = "SW";
};

struct SpiceModelCSW : public SpiceModel
{
    Param IT;   // Threshold current (A)
    Param IH;   // Hysteresis current (A)
    Param RON;  // On resistance (Ohm)
    Param ROFF; // Off resistance (Ohm)
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret = type+"(";
        if(!IT.empty()) ret.append("IT="+IT);
        if(!IH.empty()) ret.append(" IH="+IH);
        if(!RON.empty()) ret.append(" RON="+RON);
        if(!ROFF.empty()) ret.append(" ROFF="+ROFF);
        ret.append(")");
        return ret.uppercase();
    }
protected:
    Param type = "CSW";
};

struct SpiceModelLTRA : public SpiceModel
{
    Param R;            // Resistance/length
    Param L;            // Inductance/length
    Param G;            // Conductance/length
    Param C;            // Capacitance/length
    Param LEN;          // Length of line
    Param REL;          // Breakpoint control
    Param ABS;          // Breakpoint control
    Param NOSTEPLIMIT;  // Don't limit timestep to less than line delay
    Param NOCONTROL;    // Don't do complex timestep control
    Param LININTERP;    // Use linear interpolation
    Param MIXEDINTERP;  // Use linear when quadratic seems bad
    Param COMPACTREL;   // Special reltol for history compaction
    Param COMPACTABS;   // Special abstol for history compaction
    Param TRUNCNR;      // Use Newton-Raphson for timestep control
    Param TRUNCDONTCUT; // Don't limit timestep to keep impulse-response errors low
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret = type+"(";
        if(!R.empty()) ret.append("R="+R);
        if(!L.empty()) ret.append(" L="+L);
        if(!G.empty()) ret.append(" G="+G);
        if(!C.empty()) ret.append(" C="+C);
        if(!LEN.empty()) ret.append(" LEN="+LEN);
        if(!REL.empty()) ret.append(" REL="+REL);
        if(!ABS.empty()) ret.append(" ABS="+ABS);
        if(!NOSTEPLIMIT.empty()) ret.append(" NOSTEPLIMIT="+NOSTEPLIMIT);
        if(!NOCONTROL.empty()) ret.append(" NOCONTROL="+NOCONTROL);
        if(!LININTERP.empty()) ret.append(" LININTERP="+LININTERP);
        if(!MIXEDINTERP.empty()) ret.append(" MIXEDINTERP="+MIXEDINTERP);
        if(!COMPACTREL.empty()) ret.append(" COMPACTREL="+COMPACTREL);
        if(!COMPACTABS.empty()) ret.append(" COMPACTABS="+COMPACTABS);
        if(!TRUNCNR.empty()) ret.append(" TRUNCNR="+TRUNCNR);
        if(!TRUNCDONTCUT.empty()) ret.append(" TRUNCDONTCUT="+TRUNCDONTCUT);
        ret.append(")");
        return ret.uppercase();
    }
protected:
    Param type = "LTRA";
};

struct SpiceModelURC : public SpiceModel
{
    Param K;        // Propagation constant
    Param FMAX;     // Maximum frequency of interest
    Param RPERL;    // Resistance per unit length
    Param CPERL;    // Capacitance per unit length
    Param ISPERL;   // Saturation current per unit length
    Param RSPERL;   // Diode resistance per unit length
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret = type+"(";
        if(!K.empty()) ret.append("K="+K);
        if(!FMAX.empty()) ret.append("FMAX="+FMAX);
        if(!RPERL.empty()) ret.append("RPERL="+RPERL);
        if(!CPERL.empty()) ret.append("CPERL="+CPERL);
        if(!ISPERL.empty()) ret.append("ISPERL="+ISPERL);
        if(!RSPERL.empty()) ret.append("RSPERL="+RSPERL);
        ret.append(")");
        return ret.uppercase();
    }
protected:
    Param type = "URC";
};

struct SpiceModelD : public SpiceModel
{
    // Params
    Param IS;       // Saturation current
    Param RS;       // Ohmic resistance
    Param N;        // Emission coefficient
    Param TT;       // Transit time
    Param CJO;      // Zero-bias junction capacitance
    Param VJ;       // Junction potential
    Param M;        // Grading coefficient
    Param EG;       // Activation energy (eV)
    Param XTI;      // Saturation current temp. exp
    Param KF;       // Flicker noise coefficient
    Param AF;       // Flicker noise exponent
    Param FC;       // Coefficient for forward-bias depletion capacitance formula
    Param BV;       // Reverse breakdown voltage
    Param IBV;      // Current at breakdown voltage
    Param TNOM;     // Parameter measurement temperature (C)
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret = type+"(";
        if(!IS.empty()) ret.append("IS="+IS);
        if(!RS.empty()) ret.append("RS="+RS);
        if(!N.empty()) ret.append("N="+N);
        if(!TT.empty()) ret.append("TT="+TT);
        if(!IS.empty()) ret.append("IS="+CJO);
        if(!VJ.empty()) ret.append("VJ="+VJ);
        if(!M.empty()) ret.append("M="+M);
        if(!EG.empty()) ret.append("EG="+EG);
        if(!XTI.empty()) ret.append("XTI="+XTI);
        if(!KF.empty()) ret.append("KF="+KF);
        if(!AF.empty()) ret.append("AF="+AF);
        if(!FC.empty()) ret.append("FC="+FC);
        if(!BV.empty()) ret.append("BV="+BV);
        if(!IBV.empty()) ret.append("IBV="+IBV);
        if(!TNOM.empty()) ret.append("TNOM="+TNOM);
        ret.append(")");
        return ret.uppercase();
    }
protected:
    Param type = "D";
};

struct SpiceModelBJT : public SpiceModel
{
    Param type; // NPN or PNP
    Param IS;
    Param BF;
    Param NF;
    Param VAF;
    Param IKF;
    Param ISE;
    Param NE;
    Param BR;
    Param NR;
    Param VAR;
    Param IKR;
    Param ISC;
    Param NC;
    Param RB;
    Param IRB;
    Param RBM;
    Param RE;
    Param RC;
    Param CJE;
    Param VJE;
    Param MJE;
    Param TF;
    Param XTF;
    Param VTF;
    Param ITF;
    Param PTF;
    Param CJC;
    Param VJC;
    Param MJC;
    Param XCJC;
    Param TR;
    Param CJS;
    Param VJS;
    Param MJS;
    Param XTB;
    Param EG;
    Param XTI;
    Param KF;
    Param AF;
    Param FC;
    Param TNOM;
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret = type+"(";
        if(!IS.empty()) ret.append("IS="+IS);
        if(!BF.empty()) ret.append("BF="+IS);
        if(!NF.empty()) ret.append("NF="+NF);
        if(!VAF.empty()) ret.append("VAF="+VAF);
        if(!IKF.empty()) ret.append("IKF="+IKF);
        if(!ISE.empty()) ret.append("ISE="+ISE);
        if(!NE.empty()) ret.append("NE="+NE);
        if(!BR.empty()) ret.append("BR="+BR);
        if(!NR.empty()) ret.append("NR="+NR);
        if(!VAR.empty()) ret.append("VAR="+VAR);
        if(!IKR.empty()) ret.append("IKR="+IKR);
        if(!ISC.empty()) ret.append("ISC="+ISC);
        if(!NC.empty()) ret.append("NC="+NC);
        if(!RB.empty()) ret.append("RB="+RB);
        if(!IRB.empty()) ret.append("IRB="+IRB);
        if(!RBM.empty()) ret.append("RBM="+RBM);
        if(!RE.empty()) ret.append("RE="+RE);
        if(!RC.empty()) ret.append("RC="+RC);
        if(!CJE.empty()) ret.append("CJE="+CJE);
        if(!VJE.empty()) ret.append("VJE="+VJE);
        if(!MJE.empty()) ret.append("MJE="+MJE);
        if(!TF.empty()) ret.append("TF="+TF);
        if(!XTF.empty()) ret.append("XTF="+XTF);
        if(!VTF.empty()) ret.append("VTF="+VTF);
        if(!ITF.empty()) ret.append("ITF="+ITF);
        if(!PTF.empty()) ret.append("PTF="+PTF);
        if(!CJC.empty()) ret.append("CJC="+CJC);
        if(!VJC.empty()) ret.append("VJC="+VJC);
        if(!MJC.empty()) ret.append("MJC="+MJC);
        if(!XCJC.empty()) ret.append("XCJC="+XCJC);
        if(!TR.empty()) ret.append("TR="+TR);
        if(!CJS.empty()) ret.append("CJS="+CJS);
        if(!VJS.empty()) ret.append("VJS="+VJS);
        if(!MJS.empty()) ret.append("MJS="+MJS);
        if(!XTB.empty()) ret.append("XTB="+XTB);
        if(!EG.empty()) ret.append("EG="+EG);
        if(!XTI.empty()) ret.append("XTI="+XTI);
        if(!KF.empty()) ret.append("KF="+KF);
        if(!AF.empty()) ret.append("AF="+AF);
        if(!FC.empty()) ret.append("FC="+FC);
        if(!TNOM.empty()) ret.append("TNOM="+TNOM);
        ret.append(")");
        return ret.uppercase();
    }
};

struct SpiceModelJFET : public SpiceModel
{
    Param type; // NJF or PJF
    Param VTO	;
    Param BETA	;
    Param LAMBDA;
    Param RD	;
    Param RS	;
    Param CGS	;
    Param CGD	;
    Param PB	;
    Param IS	;
    Param B		;
    Param KF	;
    Param AF	;
    Param FC	;
    Param TNOM	;
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret = type+"(";
        if(!VTO.empty()) ret.append("VTO="+VTO);
        if(!BETA.empty()) ret.append("BETA="+BETA);
        if(!LAMBDA.empty()) ret.append("LAMBDA="+LAMBDA);
        if(!RD.empty()) ret.append("RD="+RD);
        if(!RS.empty()) ret.append("RS="+RS);
        if(!CGS.empty()) ret.append("CGS="+CGS);
        if(!CGD.empty()) ret.append("CGD="+CGD);
        if(!PB.empty()) ret.append("PB="+PB);
        if(!IS.empty()) ret.append("IS="+IS);
        if(!B.empty()) ret.append("B="+B);
        if(!KF.empty()) ret.append("KF="+KF);
        if(!AF.empty()) ret.append("AF="+AF);
        if(!FC.empty()) ret.append("FC="+FC);
        if(!TNOM.empty()) ret.append("TNOM="+TNOM);
        ret.append(")");
        return ret.uppercase();
    }
};
// MOS levels 1,2,3, and 6
struct SpiceModelMOSFET : public SpiceModel
{
    Param type; // NMOS or PMOS
    Param LEVEL;
    Param VTO;
    Param KP;
    Param GAMMA;
    Param PHI;
    Param LAMBDA;
    Param RD;
    Param RS;
    Param CBD;
    Param CBS;
    Param IS;
    Param PB;
    Param CGSO;
    Param CGDO;
    Param CGBO;
    Param RSH;
    Param CJ;
    Param MJ;
    Param CJSW;
    Param MJSW;
    Param JS;
    Param TOX;
    Param NSUB;
    Param NSS;
    Param NFS;
    Param TPG;
    Param XJ;
    Param LD;
    Param UO;
    Param UCRIT;
    Param UEXP;
    Param UTRA;
    Param VMAX;
    Param NEFF;
    Param KF;
    Param AF;
    Param DELTA;
    Param THETA;
    Param ETA;
    Param KAPPA;
    Param TNOM;
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret = type+"(";
        if(!LEVEL.empty()) ret.append("LEVEL="+LEVEL);
        if(!VTO.empty()) ret.append("VTO="+VTO);
        if(!KP.empty()) ret.append("KP="+KP);
        if(!GAMMA.empty()) ret.append("GAMMA="+GAMMA);
        if(!PHI.empty()) ret.append("PHI="+PHI);
        if(!LAMBDA.empty()) ret.append("LAMBDA="+LAMBDA);
        if(!RD.empty()) ret.append("RD="+RD);
        if(!RS.empty()) ret.append("RS="+RS);
        if(!CBD.empty()) ret.append("CBD="+CBD);
        if(!CBS.empty()) ret.append("CBS="+CBS);
        if(!IS.empty()) ret.append("IS="+IS);
        if(!PB.empty()) ret.append("PB="+PB);
        if(!CGSO.empty()) ret.append("CGSO="+CGSO);
        if(!CGDO.empty()) ret.append("CGDO="+CGDO);
        if(!CGBO.empty()) ret.append("CGBO="+CGBO);
        if(!RSH.empty()) ret.append("RSH="+RSH);
        if(!CJ.empty()) ret.append("CJ="+CJ);
        if(!MJ.empty()) ret.append("MJ="+MJ);
        if(!CJSW.empty()) ret.append("CJSW="+CJSW);
        if(!MJSW.empty()) ret.append("MJSW="+MJSW);
        if(!JS.empty()) ret.append("JS="+JS);
        if(!TOX.empty()) ret.append("TOX="+TOX);
        if(!NSUB.empty()) ret.append("NSUB="+NSUB);
        if(!NSS.empty()) ret.append("NSS="+NSS);
        if(!NFS.empty()) ret.append("NFS="+NFS);
        if(!TPG.empty()) ret.append("TPG="+TPG);
        if(!XJ.empty()) ret.append("XJ="+XJ);
        if(!LD.empty()) ret.append("LD="+LD);
        if(!UO.empty()) ret.append("UO="+UO);
        if(!UCRIT.empty()) ret.append("UCRIT="+UCRIT);
        if(!UEXP.empty()) ret.append("UEXP="+UEXP);
        if(!UTRA.empty()) ret.append("UTRA="+UTRA);
        if(!VMAX.empty()) ret.append("VMAX="+VMAX);
        if(!NEFF.empty()) ret.append("NEFF="+NEFF);
        if(!KF.empty()) ret.append("KF="+KF);
        if(!AF.empty()) ret.append("AF="+AF);
        if(!DELTA.empty()) ret.append("DELTA="+DELTA);
        if(!THETA.empty()) ret.append("THETA="+THETA);
        if(!ETA.empty()) ret.append("ETA="+ETA);
        if(!KAPPA.empty()) ret.append("KAPPA="+KAPPA);
        if(!TNOM.empty()) ret.append("TNOM="+TNOM);
        ret.append(")");
        return ret.uppercase();
    }
};

// MOS level 4
struct SpiceModelMOSFETBSIM : public SpiceModel
{
    Param type; // NMOS or PMOS
    Param VFB	;
    Param PHI	;
    Param K1	;
    Param K2	;
    Param ETA	;
    Param MUZ	;
    Param DL	;
    Param DW	;
    Param U0	;
    Param U1	;
    Param X2MZ	;
    Param X2E	;
    Param X3E	;
    Param X2U0	;
    Param X2U1	;
    Param MUS	;
    Param X2MS	;
    Param X3MS	;
    Param X3U1	;
    Param TOX	;
    Param TEMP	;
    Param VDD	;
    Param CGDO	;
    Param CGSO	;
    Param CGBO	;
    Param XPART	;
    Param N0	;
    Param NB	;
    Param ND	;
    Param RSH	;
    Param JS	;
    Param PB	;
    Param MJ	;
    Param PBSW	;
    Param MJSW	;
    Param CJ	;
    Param CJSW	;
    Param WDF	;
    Param DELL	;
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret = type+"(";
        if(!VFB.empty()) ret.append("VFB="+VFB);
        if(!PHI.empty()) ret.append("PHI="+PHI);
        if(!K1.empty()) ret.append("K1="+K1);
        if(!K2.empty()) ret.append("K2="+K2);
        if(!ETA.empty()) ret.append("ETA="+ETA);
        if(!MUZ.empty()) ret.append("MUZ="+MUZ);
        if(!DL.empty()) ret.append("DL="+DL);
        if(!DW.empty()) ret.append("DW="+DW);
        if(!U0.empty()) ret.append("U0="+U0);
        if(!U1.empty()) ret.append("U1="+U1);
        if(!X2MZ.empty()) ret.append("X2MZ="+X2MZ);
        if(!X2E.empty()) ret.append("X2E="+X2E);
        if(!X3E.empty()) ret.append("X3E="+X3E);
        if(!X2U0.empty()) ret.append("X2U0="+X2U0);
        if(!X2U1.empty()) ret.append("X2U1="+X2U1);
        if(!MUS.empty()) ret.append("MUS="+MUS);
        if(!X2MS.empty()) ret.append("X2MS="+X2MS);
        if(!X3MS.empty()) ret.append("X3MS="+X3MS);
        if(!X3U1.empty()) ret.append("X3U1="+X3U1);
        if(!TOX.empty()) ret.append("TOX="+TOX);
        if(!TEMP.empty()) ret.append("TEMP="+TEMP);
        if(!VDD.empty()) ret.append("VDD="+VDD);
        if(!CGDO.empty()) ret.append("CGDO="+CGDO);
        if(!CGSO.empty()) ret.append("CGSO="+CGSO);
        if(!CGBO.empty()) ret.append("CGBO="+CGBO);
        if(!XPART.empty()) ret.append("XPART="+XPART);
        if(!N0.empty()) ret.append("N0="+N0);
        if(!NB.empty()) ret.append("NB="+NB);
        if(!ND.empty()) ret.append("ND="+ND);
        if(!RSH.empty()) ret.append("RSH="+RSH);
        if(!JS.empty()) ret.append("JS="+JS);
        if(!PB.empty()) ret.append("PB="+PB);
        if(!MJ.empty()) ret.append("MJ="+MJ);
        if(!PBSW.empty()) ret.append("PBSW="+PBSW);
        if(!MJSW.empty()) ret.append("MJSW="+MJSW);
        if(!CJ.empty()) ret.append("CJ="+CJ);
        if(!CJSW.empty()) ret.append("CJSW="+CJSW);
        if(!WDF.empty()) ret.append("WDF="+WDF);
        if(!DELL.empty()) ret.append("DELL="+DELL);
        ret.append(")");
        return ret.uppercase();
    }
};

struct SpiceModelMESFET : public SpiceModel
{
    Param type; // NMF or PMF
    Param VTO	;
    Param BETA	;
    Param B		;
    Param ALPHA	;
    Param LAMBDA;
    Param RD	;
    Param RS	;
    Param CGS	;
    Param CGD	;
    Param PB	;
    Param KF	;
    Param AF	;
    Param FC	;
    virtual Glib::ustring get_spice_line()
    {
        Glib::ustring ret = type+"(";
        if(!VTO.empty()) ret.append("VTO="+VTO);
        if(!BETA.empty()) ret.append("BETA="+BETA);
        if(!B.empty()) ret.append("B="+B);
        if(!ALPHA.empty()) ret.append("ALPHA="+ALPHA);
        if(!LAMBDA.empty()) ret.append("LAMBDA="+LAMBDA);
        if(!RD.empty()) ret.append("RD="+RD);
        if(!RS.empty()) ret.append("RS="+RS);
        if(!CGS.empty()) ret.append("CGS="+CGS);
        if(!CGD.empty()) ret.append("CGD="+CGD);
        if(!PB.empty()) ret.append("PB="+PB);
        if(!KF.empty()) ret.append("KF="+KF);
        if(!AF.empty()) ret.append("AF="+AF);
        if(!FC.empty()) ret.append("FC="+FC);
        ret.append(")");
        return ret.uppercase();
    }
};



#endif /* SPICE_MODEL_H */
