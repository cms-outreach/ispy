<xsl:transform
               xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
               version="1.0">
  <xsl:output method="text" indent="no"/>
  <xsl:template match="text()"><xsl:value-of select="normalize-space(.)"/></xsl:template>
  <xsl:template match="*"></xsl:template>
  <xsl:template match="PROCESS-LIST">Profiling Results:
<xsl:apply-templates/></xsl:template>
  <xsl:template match="PROCESS"><xsl:apply-templates/></xsl:template>
  <xsl:template match="SYMBOL"><xsl:value-of select="@HITS"/><xsl:text> </xsl:text><xsl:value-of select="@NAME"/><xsl:text>
</xsl:text>
</xsl:template>
</xsl:transform>
