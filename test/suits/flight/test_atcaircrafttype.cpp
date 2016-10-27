
#include "test_atcaircrafttype.h"

Test_ATCAircraftType::Test_ATCAircraftType()
{
    ATCPaths paths;
    APFpath = paths.APF_TEST_PATH;
    OPFpath = paths.OPF_TEST_PATH;
}

Test_ATCAircraftType::~Test_ATCAircraftType()
{

}

void Test_ATCAircraftType::test_constructObject()
{
    ATCAircraftType foo(OPFpath, APFpath);

    QVERIFY(foo.getAcType().ICAOcode == "B738");
    QVERIFY(foo.getAcType().engineCount == 2);
    QVERIFY(foo.getAcType().engineType == ATC::Jet);
    QVERIFY(foo.getAcType().wake == ATC::M);

    QVERIFY(foo.getMass().ref == .65300E+02);
    QVERIFY(foo.getMass().min == .41150E+02);
    QVERIFY(foo.getMass().max == .78300E+02);
    QVERIFY(foo.getMass().payload == .20300E+02);
    QVERIFY(foo.getMass().gradient == .26974E+00);

    QVERIFY(foo.getEnvelope().VMO == .34000E+03);
    QVERIFY(foo.getEnvelope().MMO == .82000E+00);
    QVERIFY(foo.getEnvelope().h_max == .41000E+05);
    QVERIFY(foo.getEnvelope().h_max_MTOW == .34982E+05);
    QVERIFY(foo.getEnvelope().tempGradient == -.5410E+02);

    QVERIFY(foo.getSurface() == .12465E+03);

    QVERIFY(foo.getBuffeting().Clbo == .12454E+01);
    QVERIFY(foo.getBuffeting().K == .55567E+00);
    QVERIFY(foo.getBuffeting().CM16 == .00000E+00);

    QVERIFY(foo.getAeroCR().V_stall == .14900E+03);
    QVERIFY(foo.getAeroCR().CD0 == .25452E-01);
    QVERIFY(foo.getAeroCR().CD2 == .35815E-01);

    QVERIFY(foo.getAeroIC().V_stall == .11900E+03);
    QVERIFY(foo.getAeroIC().CD0 == .26200E-01);
    QVERIFY(foo.getAeroIC().CD2 == .44800E-01);

    QVERIFY(foo.getAeroTO().V_stall == .11700E+03);
    QVERIFY(foo.getAeroTO().CD0 == .35700E-01);
    QVERIFY(foo.getAeroTO().CD2 == .42300E-01);

    QVERIFY(foo.getAeroAP().V_stall == .10900E+03);
    QVERIFY(foo.getAeroAP().CD0 == .49200E-01);
    QVERIFY(foo.getAeroAP().CD2 == .42400E-01);

    QVERIFY(foo.getAeroLD().V_stall == .10700E+03);
    QVERIFY(foo.getAeroLD().CD0 == .68900E-01);
    QVERIFY(foo.getAeroLD().CD2 == .40400E-01);

    QVERIFY(foo.getCDldg() == .24900E-01);

    QVERIFY(foo.getThrust().C_Tc1 == .14659E+06);
    QVERIFY(foo.getThrust().C_Tc2 == .53872E+05);
    QVERIFY(foo.getThrust().C_Tc3 == .30453E-10);
    QVERIFY(foo.getThrust().C_Tc4 == .96177E+01);
    QVERIFY(foo.getThrust().C_Tc5 == .85132E-02);

    QVERIFY(foo.getThrust().C_Tdes_low == .75573E-01);
    QVERIFY(foo.getThrust().C_Tdes_high == .87412E-01);
    QVERIFY(foo.getThrust().h_pdes == .95770E+04);
    QVERIFY(foo.getThrust().C_Tdes_app == .19448E+00);
    QVERIFY(foo.getThrust().C_Tdes_ldg == .30610E+00);

    QVERIFY(foo.getFuel().C_f1 == .70057E+00);
    QVERIFY(foo.getFuel().C_f2 == .10681E+04);
    QVERIFY(foo.getFuel().C_f3 == .14190E+02);
    QVERIFY(foo.getFuel().C_f4 == .65932E+05);
    QVERIFY(foo.getFuel().C_fcr == .92958E+00);

    QVERIFY(foo.getGround().TOL == .23080E+04);
    QVERIFY(foo.getGround().LDL == .16460E+04);
    QVERIFY(foo.getGround().span == .34300E+02);
    QVERIFY(foo.getGround().length == .39470E+02);

    QVERIFY(foo.getVelocity().V_CL1_LO == 301);
    QVERIFY(foo.getVelocity().V_CL2_LO == 302);
    QVERIFY(foo.getVelocity().M_CL_LO == 0.78);
    QVERIFY(foo.getVelocity().V_CR1_LO == 281);
    QVERIFY(foo.getVelocity().V_CR2_LO == 282);
    QVERIFY(foo.getVelocity().M_CR_LO == 0.70);
    QVERIFY(foo.getVelocity().V_DS1_LO == 291);
    QVERIFY(foo.getVelocity().V_DS2_LO == 292);
    QVERIFY(foo.getVelocity().M_DS_LO == 0.81);

    QVERIFY(foo.getVelocity().V_CL1_AV == 303);
    QVERIFY(foo.getVelocity().V_CL2_AV == 304);
    QVERIFY(foo.getVelocity().M_CL_AV == 0.79);
    QVERIFY(foo.getVelocity().V_CR1_AV == 283);
    QVERIFY(foo.getVelocity().V_CR2_AV== 284);
    QVERIFY(foo.getVelocity().M_CR_AV == 0.71);
    QVERIFY(foo.getVelocity().V_DS1_AV == 293);
    QVERIFY(foo.getVelocity().V_DS2_AV == 294);
    QVERIFY(foo.getVelocity().M_DS_AV == 0.82);

    QVERIFY(foo.getVelocity().V_CL1_HI == 305);
    QVERIFY(foo.getVelocity().V_CL2_HI == 306);
    QVERIFY(foo.getVelocity().M_CL_HI == 0.80);
    QVERIFY(foo.getVelocity().V_CR1_HI == 285);
    QVERIFY(foo.getVelocity().V_CR2_HI == 286);
    QVERIFY(foo.getVelocity().M_CR_HI == 0.72);
    QVERIFY(foo.getVelocity().V_DS1_HI == 295);
    QVERIFY(foo.getVelocity().V_DS2_HI == 296);
    QVERIFY(foo.getVelocity().M_DS_HI == 0.83);
}

