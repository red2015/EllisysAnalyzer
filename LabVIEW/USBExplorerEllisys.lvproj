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
			<Item Name="FindAnalyzer.vi" Type="VI" URL="../FindAnalyzer.vi"/>
			<Item Name="GetSerialNumber.vi" Type="VI" URL="../GetSerialNumber.vi"/>
		</Item>
		<Item Name="ChartRectangle" Type="Folder">
			<Item Name="RectangleStuff" Type="Folder">
				<Item Name="HightRectangle.vi" Type="VI" URL="../HightRectangle.vi"/>
				<Item Name="WidthRectagle.vi" Type="VI" URL="../WidthRectagle.vi"/>
			</Item>
			<Item Name="Chart.vi" Type="VI" URL="../Chart.vi"/>
			<Item Name="ConstBottomOfChart.vi" Type="VI" URL="../ConstBottomOfChart.vi"/>
			<Item Name="DrawLine.vi" Type="VI" URL="../DrawLine.vi"/>
			<Item Name="FillRectangleForChart.vi" Type="VI" URL="../FillRectangleForChart.vi"/>
			<Item Name="InitChartFrameIn.vi" Type="VI" URL="../InitChartFrameIn.vi"/>
		</Item>
		<Item Name="TestFindingAnalyzer" Type="Folder">
			<Item Name="Tests.vi" Type="VI" URL="../Tests.vi"/>
		</Item>
		<Item Name="Tests" Type="Folder">
			<Item Name="TestChar2.vi" Type="VI" URL="../TestChar2.vi"/>
			<Item Name="TestChart.vi" Type="VI" URL="../TestChart.vi"/>
		</Item>
		<Item Name="piechart.vi" Type="VI" URL="../piechart.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="Draw Arc.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Draw Arc.vi"/>
				<Item Name="Draw Line.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Draw Line.vi"/>
				<Item Name="Draw Rectangle.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Draw Rectangle.vi"/>
				<Item Name="Empty Picture" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Empty Picture"/>
				<Item Name="Move Pen.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Move Pen.vi"/>
				<Item Name="Set Pen State.vi" Type="VI" URL="/&lt;vilib&gt;/picture/picture.llb/Set Pen State.vi"/>
			</Item>
			<Item Name="USBAnalyzerEllisysDll.dll" Type="Document" URL="../../USBAnalyzerEllisysDll/Debug/USBAnalyzerEllisysDll.dll"/>
		</Item>
		<Item Name="Build Specifications" Type="Build"/>
	</Item>
</Project>
