#ifndef CONSTANTS_H
#define CONSTANTS_H

// define your own namespace to hold constants
namespace constants
{
    inline constexpr double eta_e_cut { 2.5 }; // note: now inline constexpr
    inline constexpr double eta_m_cut { 2.4 };
    inline constexpr double eta_j_cut { 2.4 };
    inline constexpr double pres_e_pt_cut { 7.0 };
    inline constexpr double pres_m_pt_cut { 5.0 };
    inline constexpr double dxy_cut { 0.05 };
    inline constexpr double dz_cut { 0.1 };
    inline constexpr double iso_cut { 0.4 };
    inline constexpr double sip3d_cut { 8 };
    inline constexpr double btag_wp_medium_UL17 { 0.3040 };
    inline constexpr double btag_wp_loose_UL17 { 0.0532 };
    inline constexpr double fo_pt_cut { 10.0 };
    inline constexpr double mva_TTH_e_cut { 0.90 };
    inline constexpr double mva_TTH_m_cut { 0.85 };
    inline constexpr double fo_e_jetRelIso_cut { 1.0 };
    inline constexpr double fo_m_jetRelIso_cut { 0.5 };
    inline constexpr int jet_id_cut { 0 };
}
#endif
