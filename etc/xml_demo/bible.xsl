<?xml version="1.0" ?> 
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:template match="/">
<html>
<body>
<h2>Multi-script XML Example</h2> 
<xsl:apply-templates /> 
</body>
</html>
</xsl:template>


<xsl:template match="BIBLE">
<p>
<h3>Korean</h3>
<font color="red">
<xsl:value-of select="한국어" /> 
</font>
</p>

<p>
<h3>Chinese</h3>
<font color="green">
<xsl:value-of select="中國語" /> 
</font>
</p>

<p>
<h3>Japanese</h3>
<font color="blue">
<xsl:value-of select="日本語" /> 
</font>
</p>


</xsl:template>

</xsl:stylesheet>