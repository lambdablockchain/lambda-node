# Copyright (c) 2018-2021 The Lambda developers

project(lambda-qt-locale)

include(BrewHelper)
find_brew_prefix(QT5_PREFIX qt5)

find_package(Qt5LinguistTools HINTS "${QT5_PREFIX}")

if(Qt5LinguistTools_VERSION VERSION_GREATER_EQUAL 5.11)
	set(QT5_ADD_TRANSLATION_OPTIONS OPTIONS -silent)
endif()

qt5_add_translation(LAMBDA_QM_FILES
	lambda_af.ts
	lambda_af_ZA.ts
	lambda_ar.ts
	lambda_be_BY.ts
	lambda_bg_BG.ts
	lambda_bg.ts
	lambda_ca_ES.ts
	lambda_ca.ts
	lambda_ca@valencia.ts
	lambda_cs.ts
	lambda_cy.ts
	lambda_da.ts
	lambda_de.ts
	lambda_el_GR.ts
	lambda_el.ts
	lambda_en_GB.ts
	lambda_en.ts
	lambda_eo.ts
	lambda_es_AR.ts
	lambda_es_CL.ts
	lambda_es_CO.ts
	lambda_es_DO.ts
	lambda_es_ES.ts
	lambda_es_MX.ts
	lambda_es.ts
	lambda_es_UY.ts
	lambda_es_VE.ts
	lambda_et_EE.ts
	lambda_et.ts
	lambda_eu_ES.ts
	lambda_fa_IR.ts
	lambda_fa.ts
	lambda_fi.ts
	lambda_fr_CA.ts
	lambda_fr_FR.ts
	lambda_fr.ts
	lambda_gl.ts
	lambda_he.ts
	lambda_hi_IN.ts
	lambda_hr.ts
	lambda_hu.ts
	lambda_id_ID.ts
	lambda_it.ts
	lambda_ja.ts
	lambda_ka.ts
	lambda_kk_KZ.ts
	lambda_ko_KR.ts
	lambda_ku_IQ.ts
	lambda_ky.ts
	lambda_la.ts
	lambda_lt.ts
	lambda_lv_LV.ts
	lambda_mk_MK.ts
	lambda_mn.ts
	lambda_ms_MY.ts
	lambda_nb.ts
	lambda_ne.ts
	lambda_nl.ts
	lambda_pam.ts
	lambda_pl.ts
	lambda_pt_BR.ts
	lambda_pt_PT.ts
	lambda_ro_RO.ts
	lambda_ro.ts
	lambda_ru_RU.ts
	lambda_ru.ts
	lambda_sk.ts
	lambda_sl_SI.ts
	lambda_sq.ts
	lambda_sr@latin.ts
	lambda_sr.ts
	lambda_sv.ts
	lambda_sw.ts
	lambda_ta.ts
	lambda_th_TH.ts
	lambda_tr_TR.ts
	lambda_tr.ts
	lambda_uk.ts
	lambda_ur_PK.ts
	lambda_uz@Cyrl.ts
	lambda_vi.ts
	lambda_vi_VN.ts
	lambda_zh_HK.ts
	lambda_zh.ts
	lambda_zh_TW.ts
	${QT5_ADD_TRANSLATION_OPTIONS}
)

add_custom_target(locales DEPENDS ${LAMBDA_QM_FILES})
