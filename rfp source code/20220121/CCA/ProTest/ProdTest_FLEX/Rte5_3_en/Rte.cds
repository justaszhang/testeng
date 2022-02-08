<distribution version="17.0.0" name="Rte" type="MSI">
	<prebuild>
		<workingdir>workspacedir</workingdir>
		<actions></actions></prebuild>
	<postbuild>
		<workingdir>workspacedir</workingdir>
		<actions></actions></postbuild>
	<msi GUID="{5D892CA8-E169-4590-A872-8B663490E6FF}">
		<general appName="Rte" outputLocation="g:\ProTest\ProdTest_FLEX\Rte5_3_en\cvidistkit.Rte" relOutputLocation="cvidistkit.Rte" outputLocationWithVars="g:\ProTest\ProdTest_FLEX\Rte5_3_en\cvidistkit.%name" relOutputLocationWithVars="cvidistkit.%name" upgradeBehavior="1" autoIncrement="false" version="1.0.8">
			<arp company="Mitel" companyURL="" supportURL="" contact="" phone="" comments=""/>
			<summary title="" subject="" keyWords="" comments="" author=""/></general>
		<userinterface language="English" showPaths="true" showRuntimeOnly="true" readMe="" license="">
			<dlgstrings welcomeTitle="Rte" welcomeText=""/></userinterface>
		<dirs appDirID="113">
			<installDir name="AdapterCode" dirID="100" parentID="103" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="Rte" dirID="101" parentID="7" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="ProTest" dirID="102" parentID="109" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="ProdTest_FLEX" dirID="103" parentID="102" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="FUNCTION" dirID="104" parentID="108" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="COMMON" dirID="105" parentID="103" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="Projekte" dirID="106" parentID="103" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="[Start&gt;&gt;Programs]" dirID="7" parentID="-1" isMSIDir="true" visible="true" unlock="false"/>
			<installDir name="C:" dirID="107" parentID="-1" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="[Program Files]" dirID="2" parentID="-1" isMSIDir="true" visible="true" unlock="false"/>
			<installDir name="RFP_IP_4x" dirID="108" parentID="106" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="TestArea" dirID="109" parentID="107" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="Rte5_3_en" dirID="110" parentID="103" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="wwwroot" dirID="111" parentID="112" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="DHCP_Server" dirID="112" parentID="107" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="Rte" dirID="113" parentID="2" isMSIDir="false" visible="true" unlock="false"/></dirs>
		<files>
			<simpleFile fileID="0" sourcePath="G:\ProTest\ProdTest_FLEX\Projekte\RFP_IP_4x\RFP_4x.PRD" targetDir="108" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="1" sourcePath="G:\ProTest\ProdTest_FLEX\Projekte\RFP_IP_4x\FUNCTION\Rfp_IP35_Function.lib" targetDir="104" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="2" sourcePath="C:\DHCP_Server\wwwroot\flash-image.bin" targetDir="111" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="3" sourcePath="C:\DHCP_Server\dhcpsrv.exe" targetDir="112" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="4" sourcePath="C:\DHCP_Server\dhcpwiz.exe" targetDir="112" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="5" sourcePath="C:\DHCP_Server\desktop.ini" targetDir="112" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="6" sourcePath="C:\DHCP_Server\wwwroot\RJ45_52.JPG" targetDir="111" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="7" sourcePath="C:\DHCP_Server\wwwroot\favicon.ico" targetDir="111" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="8" sourcePath="C:\DHCP_Server\dhcptrc.txt" targetDir="112" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="9" sourcePath="C:\DHCP_Server\wwwroot\fitImage" targetDir="111" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="10" sourcePath="C:\DHCP_Server\wwwroot\dhcppkgstyle.xsl" targetDir="111" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="11" sourcePath="C:\DHCP_Server\wwwroot\RJ45_5.JPG" targetDir="111" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="12" sourcePath="C:\DHCP_Server\dhcptrc.bak" targetDir="112" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="13" sourcePath="C:\DHCP_Server\wwwroot\release.inf" targetDir="111" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="14" sourcePath="C:\DHCP_Server\readme.txt" targetDir="112" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="15" sourcePath="C:\DHCP_Server\dhcpsrv - Copy.ini" targetDir="112" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="16" sourcePath="C:\DHCP_Server\wwwroot\index.html" targetDir="111" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="17" sourcePath="C:\DHCP_Server\wwwroot\info.txt" targetDir="111" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="18" sourcePath="C:\DHCP_Server\wwwroot\dhcpstyle.xsl" targetDir="111" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="19" sourcePath="G:\ProTest\ProdTest_FLEX\Projekte\RFP_IP_4x\FUNCTION\Rfp_IP35_Function.dll" targetDir="104" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="20" sourcePath="G:\ProTest\ProdTest_FLEX\COMMON\flex.uir" targetDir="105" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="21" sourcePath="G:\ProTest\ProdTest_FLEX\Rte5_3_en\zlib1.dll" targetDir="110" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="22" sourcePath="G:\ProTest\ProdTest_FLEX\AdapterCode\ADAPTER_FLEX.TXT" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="23" sourcePath="G:\ProTest\ProdTest_FLEX\Rte5_3_en\rte.exe" targetDir="110" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="24" sourcePath="G:\ProTest\ProdTest_FLEX\Rte5_3_en\libeay32.dll" targetDir="110" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="25" sourcePath="C:\DHCP_Server\dhcpsrv2.5.2.zip" targetDir="112" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="26" sourcePath="G:\ProTest\ProdTest_FLEX\Rte5_3_en\ssh.dll" targetDir="110" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="27" sourcePath="C:\DHCP_Server\dhcpsrv.ini" targetDir="112" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="28" sourcePath="G:\ProTest\ProdTest_FLEX\Rte5_3_en\RTE_SETTINGS.TXT" targetDir="110" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="29" sourcePath="G:\ProTest\ProdTest_FLEX\Rte5_3_en\msvcr120.dll" targetDir="110" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="30" sourcePath="C:\DHCP_Server\diddi.txt" targetDir="112" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="31" sourcePath="G:\ProTest\ProdTest_FLEX\Projekte\RFP_IP_4x\FUNCTION\FKT_RFP4G.SQU" targetDir="104" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="32" sourcePath="g:\ProTest\ProdTest_FLEX\Rte5_3_en\rte_uir.uir" relSourcePath="rte_uir.uir" relSourceBase="0" targetDir="110" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/></files>
		<mergemodules/>
		<products>
			<product name="NI LabWindows/CVI Shared Runtime 2019" UC="{80D3D303-75B9-4607-9312-E5FC68E5BFD2}" productID="{2EB63900-C920-494E-9F7B-56E6B2DCBEE9}" path="C:\ProgramData\National Instruments\MDF\ProductCache\" flavorID="_full_" flavorName="Full" verRestr="false" coreVer="19.0.49152">
				<cutSoftDeps/></product></products>
		<runtimeEngine installToAppDir="false" activeXsup="false" analysis="true" cvirte="true" dotnetsup="true" instrsup="true" lowlevelsup="false" lvrt="true" netvarsup="true" rtutilsup="false">
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
				<condition>MINOS_WIN7_SP1</condition>
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
		<baselineProducts>
			<product name="NI LabWindows/CVI Shared Runtime 2019" UC="{80D3D303-75B9-4607-9312-E5FC68E5BFD2}" productID="{2EB63900-C920-494E-9F7B-56E6B2DCBEE9}" path="(None)" flavorID="_full_" flavorName="Full" verRestr="false" coreVer="19.0.49152">
				<dependencies>
					<productID>{07298686-C518-4981-9AE1-6E62273BF43A}</productID>
					<productID>{0946CDFA-C3C6-4AD5-B884-FB72701ED351}</productID>
					<productID>{0FFFDCED-4232-4312-BBDB-179F9FDAB7EA}</productID>
					<productID>{14B23AC5-B7EF-47D1-A57D-8666BAEE13C3}</productID>
					<productID>{16BC0547-DF57-48CF-8CB5-1CC0DF3B4911}</productID>
					<productID>{1C79B8BD-F5F8-4825-8BC8-78098A726AE1}</productID>
					<productID>{39CB13EF-8677-4FF8-942D-20021AFD7AB6}</productID>
					<productID>{4C351C60-5ECC-4920-AEB6-DFE60EA14283}</productID>
					<productID>{52981014-740C-430E-A83A-711186DF565B}</productID>
					<productID>{5B4D362F-8203-490C-82D1-5F607972196F}</productID>
					<productID>{60684600-163F-45D7-83DB-E247FA48D81F}</productID>
					<productID>{62418E99-C730-4D5D-96B2-B4DDB810A652}</productID>
					<productID>{64ECB814-3A6A-4E48-9D2F-D6C2EDD725B7}</productID>
					<productID>{69586F66-56DA-43BD-BB8E-A6DAE9A4DF6D}</productID>
					<productID>{6E1BCD5B-41A1-4E69-8C6E-389F57A19F00}</productID>
					<productID>{75191165-D39E-42A2-A7A2-D74AE99F8A84}</productID>
					<productID>{7B66645D-2C18-4DB8-8D3B-A783E386708E}</productID>
					<productID>{7EE28BBA-8A24-40B1-B0D0-9066A8CFA4AA}</productID>
					<productID>{866E5D39-AB76-4536-B1CA-554BC66A1900}</productID>
					<productID>{88DFF01E-7C87-4E35-B194-CB7A2E32FA6D}</productID>
					<productID>{93977567-FFEC-453C-A47F-CE30077E9F4B}</productID>
					<productID>{9B14BAA8-31A5-463B-88C8-EC81F67BEE57}</productID>
					<productID>{AFC999BB-F270-46EF-B748-AE755EC75322}</productID>
					<productID>{C0A9F5E2-DCD5-44C1-8B03-C560F4C06D6C}</productID>
					<productID>{C1771155-665A-4C7F-B85D-D4AACF786223}</productID>
					<productID>{CE61F080-FB64-4F6C-9763-A3060A0E59E6}</productID>
					<productID>{D1027BA0-C959-44E2-B4FA-10386404AF75}</productID>
					<productID>{D72C00A8-14F3-4E45-AFAC-5A71F833E210}</productID>
					<productID>{E12F09FF-07B3-4677-8D5C-BDD01E9A5545}</productID>
					<productID>{EEA5281C-7DA9-492E-8F6F-4127DC67AAD6}</productID>
					<productID>{EFC648C5-F3BC-4096-9AFE-23121EF06828}</productID>
					<productID>{F2EEE63A-0DCA-41BF-A243-4E4C0DFA38A4}</productID>
					<productID>{F456DB40-F5DF-45FE-A582-83DBED5D6432}</productID>
					<productID>{FB67912C-BB8D-4FE2-B64E-7712EB90DEA4}</productID></dependencies></product></baselineProducts>
		<Projects NumProjects="1">
			<Project000 ProjectID="0" ProjectAbsolutePath="g:\ProTest\ProdTest_FLEX\Rte5_3_en\Rte.prj" ProjectRelativePath="Rte.prj"/></Projects>
		<buildData progressBarRate="1.056943017134333">
			<progressTimes>
				<Begin>0.000000000000000</Begin>
				<ProductsAdded>0.196219689854622</ProductsAdded>
				<DPConfigured>2.932474263589381</DPConfigured>
				<DPMergeModulesAdded>5.496156334096908</DPMergeModulesAdded>
				<DPClosed>8.789904330812929</DPClosed>
				<DistributionsCopied>8.953411185173509</DistributionsCopied>
				<End>94.612479933996681</End></progressTimes></buildData>
	</msi>
</distribution>
