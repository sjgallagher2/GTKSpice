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

#ifndef SPICE_DIRECTIVE_H
#define SPICE_DIRECTIVE_H

#include <gtkmm.h>
#include <string>

/* SPICE Directives parent class
 * 
 * Directives include:
 *  Models
 *  Subcircuits
 *  Analyses
 *  Output Control
 * 
 * Specific directives:
 *  TITLE       Title of document (must be first line in deck)
 *  .END        Last directive
 *  .OPTION     Simulator variables
 *  .MODEL      Model definition
 *  .INCLUDE    Include a file
 *  .SUBCKT     Subcircuit definition
 *  .ENDS       Subcircuit definition end
 *  .OP         Operating point analysis
 *  .DC         DC transfer function
 *  .AC         AC analysis
 *  .TRAN       Transient
 *  .IC         Initial conditions
 *  .PZ         Pole-Zero analysis
 *  .SENS       DC or small-signal AC sensitivity analysis
 *  .TF         Transfer function analysis
 *  .NODESET    Intial node voltage guesses
 *  .DISTO      Distortion analysis
 *  .NOISE      Noise analysis
 *  .PRINT      Print tabular listing of 1-8 output variables
 *  .PLOT       Define contents of one plot of 1-8 output variables
 *  .FOUR       Fourier analysis of transient analysis output
 *  .SAVE       Specify vectors to save (instead of all node voltages etc)
 * ********* NGSPICE DIRECTIVES ************
 *  .GLOBAL     Define nodes which are available to all circuit and subcircuit blocks
 *  .LIB        Include library descriptions into the input file
 *  .PARAM      Parametric netlists
 *  .TEMP       Set circuit temperature in deg C
 *  .IF         If statement
 *  .ELSE<IF>   Else (elseif) statement
 *  .PSS        Periodic steady-state analysis
 *  .PROBE      Name vectors to be saved in raw file (same as .SAVE)
 *  .MEAS       Analyze output data of tran, ac, or dc simulation
 *  .CSPARAM    Create a constant vector from a parameter in plot const
 *  .FUNC       Defines a function
 * 
 * Brace expressions are also available, in .model lines and device lines
 */



struct SpiceDirective
{
    typedef Glib::ustring Datafield;

protected:
    Datafield name;
public:
    virtual Glib::ustring get_spice_line() {return name.uppercase();}
};

/* A single string placed into a SPICE deck by the user, consisting
 * of as many SPICE lines as the user wants
 */
struct SpiceLines
{
    Glib::ustring line;
    virtual Glib::ustring get_spice_line() {return line.uppercase();}
};

/* @form: .END   */
struct SpiceEndDirective : public SpiceDirective 
{
    virtual Glib::ustring get_spice_line() {return name.uppercase();}
protected:
    Datafield name = ".END";
};

/* @form: <TITLE>   */
struct SpiceTitleDirective : public SpiceDirective 
{
    Datafield title;
    virtual Glib::ustring get_spice_line() {return title.uppercase();}

};

/* @form * <COMMENT> */
struct SpiceComment
{
    Glib::ustring comment;
    Glib::ustring get_spice_line() {return ("* "+comment).uppercase();}
};

/* @form: .MODEL MNAME TYPE(PNAME1=PVAL1 PNAME2=PVAL2 ... )
 * @notes 
 *  For pasting in a .MODEL line, use SpiceLine
 *  For parametric models, use SpiceModel to create the string
 */
struct SpiceModelDirective : public SpiceDirective 
{
public:
    Datafield modelname;
    Datafield model; // See SpiceModel in spice_model.h
    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name+" "+modelname+" "+model;
        return ret.uppercase();
    }
protected:
    Datafield name = ".MODEL";
};


/* @form: .OPTION OPT1 OPT2 ... (or OPT=OPTVAL)   */
struct SpiceOptionDirective : public SpiceDirective 
{
public:
    // Resets the current error tolerance of the program. Default is 1pA.
    Datafield abstol;

    // Use the older version of the MOS3 model with the "kappa" discontinuity
    Datafield badmos3;      // Note: No "="
    
    // Resets the charge tolerance of the program. Default is 1.0e-14. 
    Datafield chgtol;       
    
    // Resets the value for MOS drain diffusion area; default is 0.0 
    Datafield defad;        
    
    // Resets the value for MOS source diffusion area; default is 0.0 
    Datafield defas;        
    
    // Resets the value for MOS channel length; default is 100.0 um 
    Datafield defl;         
    
    // Resets the value for MOS channel width; default is 100.0 um 
    Datafield defw;         
    
    // Resets the value of GMIN, the minimum conductance allowed by the program.
    Datafield gmin;         
    
    // Resets the DC iteration limit. Default is 100. 
    Datafield itl1;         
    
    // Resets the DC transfer curve itration limit. Default is 50. 
    Datafield itl2;         
    
    // Resets the lower transient analysis iteration limit. 
    // Default is 4. (Not implemented in Spice3) 
    Datafield itl3;         
    
    // Resets the transient analysis timepoint iteration limit. 
    // Default is 10. 
    Datafield itl4;         
    
    // Resets the transient analysis total iteration limit. 
    // Default is 5000. Set ITL5=0 to omit. (Not implemented in Spice3) 
    Datafield itl5;         
    
    // Retain the operating point information when either an AC, Distortion, 
    // or PZ analysis is run
    Datafield keepopinfo;   // Note: No "="
    
    // Sets the numerical integration method. Possible names are "Gear" or 
    // "trapezoidal" ("trap"). Default is "trap". 
    Datafield method;       
    
    // Resets the relative ratio betweent he largest column entry and an acceptable 
    // pivot value. Default is 1.0e-3. 
    Datafield pivrel;       
    
    // Resets the absolute minimum value for a matrix entry to be accepted as a 
    // pivot. Default is 1.0e-13. 
    Datafield pivtol;       
    
    // Resets the relative error tolerance of the program. 
    // Default is 0.001 (0.1%) 
    Datafield reltol;       
    
    // Resets the operating temperature of the circuit. 
    // Default is 27C (300K). 
    Datafield temp;         
    
    // Resets the nominal temperature at which device parameters are measured. 
    // Default is 27C (300K) .
    Datafield tnom;         
    
    // Resets the transient error tolerance. 
    // Default is 7.0. 
    Datafield trtol;        
    
    // Applicable only to the LTRA model. Simulator tries to condense transmission 
    // line's history of input V and I 
    Datafield trytocompact;  // Note: No "="
    
    // Resets the absolute voltage error tolerance of the program. 
    // Default is 1uV. 
    Datafield vntol;        

    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name;
        if(!abstol.empty()) ret.append(" ABSTOL="+abstol);
        if(!badmos3.empty()) ret.append(" BADMOS3="+badmos3);
        if(!chgtol.empty()) ret.append(" CHGTOL="+chgtol);
        if(!defad.empty()) ret.append(" DEFAD="+defad);
        if(!defas.empty()) ret.append(" DEFAS="+defas);
        if(!defl.empty()) ret.append(" DEFL="+defl);
        if(!defw.empty()) ret.append(" DEFW="+defw);
        if(!gmin.empty()) ret.append(" GMIN="+gmin);
        if(!itl1.empty()) ret.append(" ITL1="+itl1);
        if(!itl2.empty()) ret.append(" ITL2="+itl2);
        //if(!itl3.empty()) ret.append(" "+itl3);
        if(!itl4.empty()) ret.append(" ITL3="+itl4);
        //if(!itl5.empty()) ret.append(" "+itl5);
        if(!keepopinfo.empty()) ret.append(" "+keepopinfo);
        if(!method.empty()) ret.append(" METHOD="+method);
        if(!pivrel.empty()) ret.append(" PIVREL="+pivrel);
        if(!pivtol.empty()) ret.append(" PIVTOL="+pivtol);
        if(!reltol.empty()) ret.append(" RELTOL="+reltol);
        if(!temp.empty()) ret.append(" TEMP="+temp);
        if(!tnom.empty()) ret.append(" TNOM="+tnom);
        if(!trtol.empty()) ret.append(" TRTOL="+trtol);
        if(!trytocompact.empty()) ret.append(" "+trytocompact);
        if(!vntol.empty()) ret.append(" VNTOL="+vntol);
        
        return ret.uppercase();
    }
protected:
    Datafield name = ".OPTION";

};
/* @form .SUBCKT SUBNAM N1 <N2 N3 ...>   
 * @notes 
 *  Nodes cannot be zero
 */
struct SpiceSubcktDirective : public SpiceDirective 
{
    Datafield subname;
    Datafield nodes;

    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name+" "+subname+" "+nodes;
        return ret.uppercase();
    }
protected:
    Datafield name = ".SUBCKT";

};

/* @form    */
struct SpiceEndsDirective : public SpiceDirective 
{
    Datafield subname;
    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name+" "+subname;
        return ret.uppercase();
    }
protected:
    Datafield name = ".ENDS";

};

/* @form: .INCLUDE filename   
 * @notes
 *  File 'filename' will be inserted into the place where the .INCLUDE line is
*/
struct SpiceIncludeDirective : public SpiceDirective 
{
    Datafield filename;
    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name+" "+filename;
        return ret.uppercase();
    }
protected:
    Datafield name = ".INCLUDE";

};

/* @form .NODESET V(NODNUM)=VAL V(NODNUM)=VAL ...   */
struct SpiceNodesetDirective : public SpiceDirective 
{
    Datafield nodevals;
    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name+" "+nodevals;
        return ret.uppercase();
    }
protected:
    Datafield name = ".NODESET";

};

/* @form .IC V(NODNUM)=VAL V(NODNUM)=VAL ...   */
struct SpiceICDirective : public SpiceDirective 
{
    Datafield nodevals;
    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name+" "+nodevals;
        return ret.uppercase();
    }
protected:
    Datafield name = ".IC";

};

/* @form .OP   */
struct SpiceOpDirective : public SpiceDirective 
{
    Glib::ustring get_spice_line()
    {
        Glib::ustring ret=name;
        return ret.uppercase();
    }
protected:
    Datafield name = ".OP";

};

/* @form .DC SRCNAM VSTART VSTOP VINCR [SRC2 START2 STOP2 INCR2]  */
struct SpiceDCDirective : public SpiceDirective 
{
    Datafield sourcename;
    Datafield vstart;
    Datafield vstop;
    Datafield vincr;

    Datafield src2;
    Datafield start2;
    Datafield stop2;
    Datafield incr2;

    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name+" "+sourcename+" "+vstart+" "+vstop+" "+vincr;
        if(!src2.empty()) ret.append(" "+src2+" "+start2+" "+stop2+" "+incr2);
        return ret.uppercase();
    }
protected:
    Datafield name = ".DC";

};

/* @form .AC DEC ND FSTART FSTOP
 * @form .AC OCT LIN FSTART FSTOP
 * @form .AC LIN NP FSTART STOP
 */
struct SpiceACDirective : public SpiceDirective 
{
    bool decade = false;
    bool octave = false;
    bool linear = false;

    Datafield dec_num_points_per_decade;
    Datafield oct_num_points_per_octave;
    Datafield lin_num_points;

    Datafield fstart;
    Datafield fstop;

    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name;
        if(decade)
            ret.append(" DEC "+dec_num_points_per_decade);
        else if(octave)
            ret.append(" OCT "+dec_num_points_per_decade);
        else if(linear)
            ret.append(" LIN "+lin_num_points);
        ret.append(" "+fstart+" "+fstop);
        return ret.uppercase();
    }
protected:
    Datafield name = ".AC";

};

/* @form .TRAN TSTEP TSTOP <TSTART <TMAX>>   */
struct SpiceTranDirective : public SpiceDirective 
{
    Datafield tstep,tstop,tstart,tmax;

    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name+" "+tstep+" "+tstop;
        if(!tstart.empty())
        {
            ret.append(" "+tstart);
            if(!tmax.empty()) ret.append(" "+tmax);
        }
        return ret.uppercase();
    }
protected:
    Datafield name = ".TRAN";

};


/* @form .PZ NODE1 NODE2 NODE3 NODE4 CUR POL
 * @form .PZ NODE1 NODE2 NODE3 NODE4 CUR ZER
 * @form .PZ NODE1 NODE2 NODE3 NODE4 CUR PZ
 * @form .PZ NODE1 NODE2 NODE3 NODE4 VOL POL
 * @form .PZ NODE1 NODE2 NODE3 NODE4 VOL ZER
 * @form .PZ NODE1 NODE2 NODE3 NODE4 VOL PZ
 */
struct SpicePZDirective : public SpiceDirective 
{
    Datafield nodes;
    bool cur,vol;
    bool pol,zer,pz;
    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name+" "+nodes;
        if(cur)
        {
            ret.append(" CUR");
            if(pol) ret.append(" CUR");
            if(zer) ret.append(" ZER"); 
            if(pz) ret.append(" PZ"); 
        }
        else if(vol)
        {
            ret.append(" VOL");
            if(pol) ret.append(" CUR");
            if(zer) ret.append(" ZER"); 
            if(pz) ret.append(" PZ"); 
        }
        return ret.uppercase();
    }
protected:
    Datafield name = ".PZ";

};

/* @form .SENS OUTVAR
 * @form .SENS OUTVAR AC DEC ND FSTART FSTOP
 * @form .SENS OUTVAR AC OCT NO FSTART FSTOP
 * @form .SENS OUTVAR AC LIN NP FSTART FSTOP
 */
struct SpiceSensDirective : public SpiceDirective 
{
    Datafield outvar;
    Datafield ac;
    bool dec;
    bool oct;
    bool lin;
    Datafield num_points_per_decade;
    Datafield num_points_per_octave;
    Datafield linear_num_points;
    Datafield fstart;
    Datafield fstop;

    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name+" "+outvar;
        if(!ac.empty())
        {
            ret.append(" "+ac);
            if(dec)
                ret.append(" DEC "+num_points_per_decade);
            else if(oct)
                ret.append(" OCT "+num_points_per_octave);
            else if(lin)
                ret.append(" LIN "+linear_num_points);
            ret.append(" "+fstart+" "+fstop);
        }
        return ret.uppercase();
    }
protected:
    Datafield name = ".SENS";

};

/* @form: .TF OUTVAR INSRC   */
struct SpiceTFDirective : public SpiceDirective 
{
    Datafield outvar;
    Datafield insrc;
    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name+" "+outvar+" "+insrc;
        return ret.uppercase();
    }
protected:
    Datafield name = ".TF";

};

/* @form .DISTO DEC ND FSTART FSTOP <F2OVERF1>
 * @form .DISTO OCT NO FSTART FSTOP <F2OVERF1>
 * @form .DISTO LIN NP FSTART FSTOP <F2OVERF1>
   */
struct SpiceDistDirective : public SpiceDirective 
{
    bool dec;
    bool oct;
    bool lin;
    Datafield num_points_per_decade;
    Datafield num_points_per_octave;
    Datafield linear_num_points;
    Datafield fstart;
    Datafield fstop;
    Datafield f2overf1;
    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name;
        if(dec)
            ret.append(" DEC "+num_points_per_decade);
        else if(oct)
            ret.append(" OCT "+num_points_per_octave);
        else if(lin)
            ret.append(" LIN "+linear_num_points);
        ret.append(" "+fstart+" "+fstop+" "+f2overf1);

        return ret.uppercase();
    }
protected:
    Datafield name = ".DIST";

};

/* @form .NOISE V(OUTPUT <,REF>) SRC ( DEC | LIN | OCT ) PTS FSTART FSTOP <PTS_PER_SUMMARY> */
struct SpiceNoiseDirective : public SpiceDirective 
{
    bool dec;
    bool oct;
    bool lin;
    Datafield vout;
    Datafield src;
    Datafield num_points_per_decade;
    Datafield num_points_per_octave;
    Datafield linear_num_points;
    Datafield fstart;
    Datafield fstop;
    Datafield points_per_summary;
    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name;
        if(dec)
            ret.append(" DEC "+num_points_per_decade);
        else if(oct)
            ret.append(" OCT "+num_points_per_octave);
        else if(lin)
            ret.append(" LIN "+linear_num_points);
        ret.append(" "+fstart+" "+fstop);
        if(!points_per_summary.empty()) ret.append(" "+points_per_summary);

        return ret.uppercase();
    }
protected:
    Datafield name = ".NOISE";

};

/* @form .PRINT PRTYPE OV1 <OV2 ... OV8>   */
struct SpicePrintDirective : public SpiceDirective 
{
    Datafield output_variables;
    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name+" "+output_variables;
        return ret.uppercase();
    }
protected:
    Datafield name = ".PRINT";

};

/* @form .PLOT PLTYPE OV1 <(PLO1, PHI1)> <OV2 <(PLO2, PHI2)> ... OV8>   */
struct SpicePlotDirective : public SpiceDirective 
{
    Datafield plottype;
    Datafield output_variables;
    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name+" "+output_variables;
        return ret.uppercase();
    }
protected:
    Datafield name = ".PLOT";

};

/* @form .FOUR FREQ OV1 <OV2 OV3 ...>   */
struct SpiceFourDirective : public SpiceDirective 
{
    Datafield output_variables;
    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name+" "+output_variables;
        return ret.uppercase();
    }
protected:
    Datafield name = ".FOUR";

};

/* @form .SAVE vector vector vector ...   */
struct SpiceSaveDirective : public SpiceDirective 
{
    Datafield vectors;
    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name+" "+vectors;
        return ret.uppercase();
    }
protected:
    Datafield name = ".SAVE";

};


/****** NGSPICE DIRECTIVES ********/
/* @form    */
struct ParamDirective : public SpiceDirective 
{
    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name;
        return ret.uppercase();
    }
protected:
    Datafield name = ".PARAM";

};
/* @form    */
struct TempDirective : public SpiceDirective 
{
    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name;
        return ret.uppercase();
    }
protected:
    Datafield name = ".TEMP";

};

/* @form    */
struct IfDirective : public SpiceDirective 
{
    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name;
        return ret.uppercase();
    }
protected:
    Datafield name = ".IF";

};

/* @form:    */
struct PSSDirective : public SpiceDirective 
{
    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name;
        return ret.uppercase();
    }
protected:
    Datafield name = ".PSS";

};
/* @form    */
struct ProbeDirective : public SpiceDirective 
{
    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name;
        return ret.uppercase();
    }
protected:
    Datafield name = ".PROBE";

};

/* @form    */
struct MeasureDirective : public SpiceDirective 
{
    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name;
        return ret.uppercase();
    }
protected:
    Datafield name = ".MEASURE";

};

/* @form    */
struct FuncDirective : public SpiceDirective 
{
    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name;
        return ret.uppercase();
    }
protected:
    Datafield name = ".FUNC";

};

/* @form    */
struct CSParamDirective : public SpiceDirective 
{
    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name;
        return ret.uppercase();
    }
protected:
    Datafield name = ".CSPARAM";

};

/* @form: .GLOBAL    */
struct GlobalDirective : public SpiceDirective 
{
    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name;
        return ret.uppercase();
    }
protected:
    Datafield name = ".GLOBAL";

};

/* @form .LIB  */
struct LibDirective : public SpiceDirective 
{
    Glib::ustring get_spice_line()
    {
        Glib::ustring ret;
        ret = name;
        return ret.uppercase();
    }
protected:
    Datafield name = ".LIB";

};



#endif /* SPICE_DIRECTIVE_H */
