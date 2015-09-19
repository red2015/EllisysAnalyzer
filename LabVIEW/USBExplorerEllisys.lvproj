<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="14008000">
	<Item Name="My Computer" Type="My Computer">
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="AnalyzerStuff" Type="Folder">
			<Item Name="Acqusiton.vi" Type="VI" URL="../Acqusiton.vi"/>
			<Item Name="DecreaseFrame.vi" Type="VI" URL="../DecreaseFrame.vi"/>
			<Item Name="FindAnalyzer.vi" Type="VI" URL="../FindAnalyzer.vi"/>
			<Item Name="GetCountOverflow.vi" Type="VI" URL="../GetCountOverflow.vi"/>
			<Item Name="GetDeviceTransactions.vi" Type="VI" URL="../GetDeviceTransactions.vi"/>
			<Item Name="GetFrameStatistics.vi" Type="VI" URL="../GetFrameStatistics.vi"/>
			<Item Name="GetSerialNumber.vi" Type="VI" URL="../GetSerialNumber.vi"/>
			<Item Name="StopAcquisition.vi" Type="VI" URL="../StopAcquisition.vi"/>
		</Item>
		<Item Name="Charts" Type="Folder">
			<Item Name="Chart stuff" Type="Folder">
				<Item Name="GetCountAllTransactions.vi" Type="VI" URL="../GetCountAllTransactions.vi"/>
				<Item Name="GetCountOfTransactionsInOutNak.vi" Type="VI" URL="../GetCountOfTransactionsInOutNak.vi"/>
				<Item Name="min(a,b).vi" Type="VI" URL="../min(a,b).vi"/>
			</Item>
			<Item Name="Example" Type="Folder">
				<Item Name="XY bar graph(1) .vi" Type="VI" URL="../XY bar graph(1) .vi"/>
				<Item Name="xy_bar_graph.vi" Type="VI" URL="../../../../Downloads/xy_bar_graph.vi"/>
				<Item Name="XyBarGraphTests.vi" Type="VI" URL="../XyBarGraphTests.vi"/>
			</Item>
			<Item Name="BarChart.vi" Type="VI" URL="../BarChart.vi"/>
			<Item Name="BarChart2.vi" Type="VI" URL="../BarChart2.vi"/>
			<Item Name="FrameChart.vi" Type="VI" URL="../FrameChart.vi"/>
			<Item Name="piechart.vi" Type="VI" URL="../piechart.vi"/>
		</Item>
		<Item Name="Pie Chart" Type="Folder">
			<Item Name="Piechart_.vi" Type="VI" URL="../Pie Chart/Piechart_.vi"/>
			<Item Name="post-4616-1160681060.vi" Type="VI" URL="../Pie Chart/post-4616-1160681060.vi"/>
			<Item Name="post-4616-1160681073.vi" Type="VI" URL="../Pie Chart/post-4616-1160681073.vi"/>
		</Item>
		<Item Name="PrintScreen stuff" Type="Folder">
			<Item Name="PathFile.vi" Type="VI" URL="../PrintScreen/PathFile.vi"/>
			<Item Name="PrintAllDevices.vi" Type="VI" URL="../PrintScreen/PrintAllDevices.vi"/>
			<Item Name="PrintAllTransactions.vi" Type="VI" URL="../PrintScreen/PrintAllTransactions.vi"/>
			<Item Name="PrintBandwidthChart.vi" Type="VI" URL="../PrintScreen/PrintBandwidthChart.vi"/>
			<Item Name="PrintFrames.vi" Type="VI" URL="../PrintScreen/PrintFrames.vi"/>
			<Item Name="PrintInOutNakTransactions.vi" Type="VI" URL="../PrintScreen/PrintInOutNakTransactions.vi"/>
			<Item Name="PrintUserData.vi" Type="VI" URL="../PrintScreen/PrintUserData.vi"/>
		</Item>
		<Item Name="Tests" Type="Folder">
			<Item Name="ChartTesting.vi" Type="VI" URL="../ChartTesting.vi"/>
			<Item Name="testVector.vi" Type="VI" URL="../testVector.vi"/>
		</Item>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="Calc Long Word Padded Width.vi" Type="VI" URL="/&lt;vilib&gt;/picture/bmp.llb/Calc Long Word Padded Width.vi"/>
				<Item Name="compatOverwrite.vi" Type="VI" URL="/&lt;vilib&gt;/_oldvers/_oldvers.llb/compatOverwrite.vi"/>
				<Item Name="Draw Arc.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Draw Arc.vi"/>
				<Item Name="Draw Oval.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Draw Oval.vi"/>
				<Item Name="Draw Text at Point.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Draw Text at Point.vi"/>
				<Item Name="Draw Text in Rect.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Draw Text in Rect.vi"/>
				<Item Name="Empty Picture" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Empty Picture"/>
				<Item Name="ex_CorrectErrorChain.vi" Type="VI" URL="/&lt;vilib&gt;/express/express shared/ex_CorrectErrorChain.vi"/>
				<Item Name="Flip and Pad for Picture Control.vi" Type="VI" URL="/&lt;vilib&gt;/picture/bmp.llb/Flip and Pad for Picture Control.vi"/>
				<Item Name="Get Text Rect.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Get Text Rect.vi"/>
				<Item Name="imagedata.ctl" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/imagedata.ctl"/>
				<Item Name="PCT Pad String.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/PCT Pad String.vi"/>
				<Item Name="RGB to Color.vi" Type="VI" URL="/&lt;vilib&gt;/Utility/colorconv.llb/RGB to Color.vi"/>
				<Item Name="Set Pen State.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Set Pen State.vi"/>
				<Item Name="subDisplayMessage.vi" Type="VI" URL="/&lt;vilib&gt;/express/express output/DisplayMessageBlock.llb/subDisplayMessage.vi"/>
				<Item Name="Write BMP Data To Buffer.vi" Type="VI" URL="/&lt;vilib&gt;/picture/bmp.llb/Write BMP Data To Buffer.vi"/>
				<Item Name="Write BMP Data.vi" Type="VI" URL="/&lt;vilib&gt;/picture/bmp.llb/Write BMP Data.vi"/>
				<Item Name="Write BMP File.vi" Type="VI" URL="/&lt;vilib&gt;/picture/bmp.llb/Write BMP File.vi"/>
			</Item>
			<Item Name="USBAnalyzerEllisysDll.dll" Type="Document" URL="../USBAnalyzerEllisysDll.dll"/>
			<Item Name="USBAnalyzerEllisysDll.dll" Type="Document" URL="../../USBAnalyzerEllisysDll/Debug/USBAnalyzerEllisysDll.dll"/>
		</Item>
		<Item Name="Build Specifications" Type="Build">
			<Item Name="My Zip File" Type="Zip File">
				<Property Name="Absolute[0]" Type="Bool">false</Property>
				<Property Name="BuildName" Type="Str">My Zip File</Property>
				<Property Name="Comments" Type="Str"></Property>
				<Property Name="DestinationID[0]" Type="Str">{BCEC828C-350A-4310-B572-7A00CECA037A}</Property>
				<Property Name="DestinationItemCount" Type="Int">1</Property>
				<Property Name="DestinationName[0]" Type="Str">Destination Directory</Property>
				<Property Name="IncludedItemCount" Type="Int">1</Property>
				<Property Name="IncludedItems[0]" Type="Ref">/My Computer</Property>
				<Property Name="IncludeProject" Type="Bool">true</Property>
				<Property Name="Path[0]" Type="Path">../../builds/USBExplorerEllisys/My Zip File/USBExplorerEllisys.zip</Property>
				<Property Name="ZipBase" Type="Str">NI_zipbasedefault</Property>
			</Item>
		</Item>
	</Item>
</Project>
