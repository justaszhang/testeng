<distribution version="13.0.0" name="DECT Tester" type="MSI">
	<prebuild>
		<workingdir>workspacedir</workingdir>
		<actions></actions></prebuild>
	<postbuild>
		<workingdir>workspacedir</workingdir>
		<actions></actions></postbuild>
	<msi GUID="{50B6541D-7E74-4A5C-B92B-468E0457699E}">
		<general appName="DECT Tester" outputLocation="c:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\cvidistkit.DECT Tester" relOutputLocation="cvidistkit.DECT Tester" outputLocationWithVars="c:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\cvidistkit.%name" relOutputLocationWithVars="cvidistkit.%name" upgradeBehavior="1" autoIncrement="false" version="1.0.0">
			<arp company="Mitel" companyURL="" supportURL="" contact="" phone="" comments=""/>
			<summary title="" subject="" keyWords="" comments="" author=""/></general>
		<userinterface language="English" showPaths="true" readMe="" license="">
			<dlgstrings welcomeTitle="DECT Tester" welcomeText=""/></userinterface>
		<dirs appDirID="101">
			<installDir name="DECT Tester" dirID="100" parentID="102" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="GUI" dirID="101" parentID="100" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="C:" dirID="102" parentID="-1" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="[Start&gt;&gt;Programs]" dirID="7" parentID="-1" isMSIDir="true" visible="true" unlock="false"/>
			<installDir name="[Desktop]" dirID="0" parentID="-1" isMSIDir="true" visible="true" unlock="false"/>
			<installDir name="DECT Tester" dirID="103" parentID="7" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="data" dirID="104" parentID="101" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="data" dirID="105" parentID="104" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="data0" dirID="106" parentID="104" isMSIDir="false" visible="true" unlock="false"/></dirs>
		<files>
			<simpleFile fileID="0" sourcePath="c:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\STOPPING.uir" relSourcePath="STOPPING.uir" relSourceBase="0" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="1" sourcePath="c:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\MAC pnl.uir" relSourcePath="MAC pnl.uir" relSourceBase="0" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="2" sourcePath="c:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\STOP BUTTON.uir" relSourcePath="STOP BUTTON.uir" relSourceBase="0" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="3" sourcePath="c:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\QUIT BUTTON.uir" relSourcePath="QUIT BUTTON.uir" relSourceBase="0" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="4" sourcePath="c:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\Serial Number pnl.uir" relSourcePath="Serial Number pnl.uir" relSourceBase="0" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="5" sourcePath="C:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\data\lvsound2.dll" targetDir="104" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="6" sourcePath="C:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\data\data\lvsound2.dll" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="7" sourcePath="C:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\data\MyDFT.dll" targetDir="104" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="8" sourcePath="C:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\data\lvsound.dll" targetDir="104" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="9" sourcePath="C:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\data\data0\lvsound2.dll" targetDir="106" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="10" sourcePath="C:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\data\General.dll" targetDir="104" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="11" sourcePath="C:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\data\Wavelet.dll" targetDir="104" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="12" sourcePath="C:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\data\lvanlys.dll" targetDir="104" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="13" sourcePath="C:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\LJUDDotNet.dll" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="14" sourcePath="C:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\AudzerLib.aliases" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="15" sourcePath="C:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\lvanlys.dll" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="16" sourcePath="C:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\lvsound.dll" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="17" sourcePath="c:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\Front Panel1.uir" relSourcePath="Front Panel1.uir" relSourceBase="0" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="18" sourcePath="C:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\AudzerLib.ini" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="19" sourcePath="C:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\sw_version.cfg" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="20" sourcePath="c:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\cvibuild.DECT_Tester\Release\DECT_Tester.exe" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="21" sourcePath="C:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\lvsound2.dll" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="22" sourcePath="C:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\AudzerLib.dll" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="23" sourcePath="c:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\MSG OK.uir" relSourcePath="MSG OK.uir" relSourceBase="0" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="24" sourcePath="c:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\MSG YES.uir" relSourcePath="MSG YES.uir" relSourceBase="0" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="25" sourcePath="C:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\Wavelet.dll" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="26" sourcePath="C:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\tester.cfg" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="27" sourcePath="C:\audio analysisl\General.dll" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="28" sourcePath="C:\audio analysisl\MyDFT.dll" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/></files>
		<fileGroups>
			<projectOutput targetType="0" dirID="101" projectID="0">
				<fileID>20</fileID></projectOutput></fileGroups>
		<mergemodules/>
		<products>
			<product name="NI LabWindows/CVI Shared Run-Time Engine" UC="{80D3D303-75B9-4607-9312-E5FC68E5BFD2}" productID="{D3C549FD-7DA5-440B-A1BC-DD92C898949A}" path="(None)" flavorID="_full_" flavorName="Full" verRestr="false" coreVer="">
				<cutSoftDeps>
					<softDep name=".NET Support" UC="{0DDB211A-941B-4125-9518-E81E10409F2E}" depKey="CVI_DotNet.CVI1320.RTE"/>
					<softDep name="Analysis Support" UC="{86208B51-159E-4F6F-9C62-0D5EFC9324D8}" depKey="CVI_Analysis.CVI1320.RTE"/>
					<softDep name="Dev (Patch 1) Support" UC="{C6F76062-CDCB-4931-8580-012AF2152602}" depKey="CVIRTEUpdated.CVI1320.RTE"/>
					<softDep name="Network Streams Support" UC="{40A5AD7F-4BAF-4A5C-8B56-426B84F75C05}" depKey="CVI_NetworkStreams.CVI1320.RTE"/>
					<softDep name="Network Variable Support" UC="{15CE39FE-1354-484D-B8CA-459077449FB3}" depKey="CVI_NetworkVariable.CVI1320.RTE"/>
					<softDep name="TDMS Support" UC="{5A8AF88D-486D-4E30-A7A5-8D8A039BBEBF}" depKey="CVI_Tdms.CVI1320.RTE"/>
					<softDep name="Visual C++ 2008 Redistributable" UC="{BD1E864A-2ACC-4F7E-9A7D-977C39952A52}" depKey="VC2008MSMs_Redist.CVI1320.RTE"/>
					<softDep name="Visual C++ 2010 Redistributable" UC="{3109870C-C165-4896-AC99-AFDD3CA3304A}" depKey="VC2010MSMs_Redist.CVI1320.RTE"/></cutSoftDeps></product>
			<product name="NI-488.2 Runtime 16.0" UC="{357F6618-C660-41A2-A185-5578CC876D1D}" productID="{060A0DD9-6B7B-4AC4-A621-A7A37D08E989}" path="(None)" flavorID="_full_" flavorName="NI-488.2" verRestr="false" coreVer="16.0.49152"/></products>
		<runtimeEngine installToAppDir="false" activeXsup="false" analysis="false" cvirte="true" dotnetsup="false" instrsup="true" lowlevelsup="false" lvrt="true" netvarsup="false" rtutilsup="false">
			<hasSoftDeps/>
			<doNotAutoSelect>
			<component>activeXsup</component>
			<component>lowlevelsup</component>
			<component>rtutilsup</component>
			</doNotAutoSelect></runtimeEngine><sxsRuntimeEngine>
			<selected>false</selected>
			<doNotAutoSelect>false</doNotAutoSelect></sxsRuntimeEngine>
		<advanced mediaSize="650">
			<launchConditions>
				<condition>MINOS_WINXP_SP0</condition>
			</launchConditions>
			<includeConfigProducts>true</includeConfigProducts>
			<maxImportVisible>silent</maxImportVisible>
			<maxImportMode>merge</maxImportMode>
			<custMsgFlag>false</custMsgFlag>
			<custMsgPath>msgrte.txt</custMsgPath>
			<signExe>false</signExe>
			<certificate></certificate>
			<signTimeURL></signTimeURL>
			<signDescURL></signDescURL></advanced>
		<Projects NumProjects="1">
			<Project000 ProjectID="0" ProjectAbsolutePath="c:\Users\zhangjus\OneDrive - Mitel Networks Corporation\Mitel\Project\DECT Phone\SourceCode\DECT_V2_Dec_04_2020_v2_0_L_Released\DECT_Tester.prj" ProjectRelativePath="DECT_Tester.prj"/></Projects>
		<buildData progressBarRate="4.485722662480949">
			<progressTimes>
				<Begin>0.000000000000000</Begin>
				<ProductsAdded>0.052584000000000</ProductsAdded>
				<DPConfigured>1.977584000000000</DPConfigured>
				<DPMergeModulesAdded>4.337584000000000</DPMergeModulesAdded>
				<DPClosed>13.162625999999998</DPClosed>
				<DistributionsCopied>13.712763999999998</DistributionsCopied>
				<End>22.292952000000000</End></progressTimes></buildData>
	</msi>
</distribution>
