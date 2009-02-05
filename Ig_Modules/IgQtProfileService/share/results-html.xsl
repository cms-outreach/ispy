<xsl:transform
               xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
               version="1.0">
  <xsl:template match="*"></xsl:template>
  <xsl:template match="PROCESS-LIST">
    <HTML>
      <HEAD>
        <TITLE>Profiling Results</TITLE>
      </HEAD>
      <BODY>
        <xsl:apply-templates/>
      </BODY>
    </HTML>
  </xsl:template>
  <xsl:template match="PROCESS">
    <TABLE>
      <xsl:apply-templates/>
    </TABLE>
  </xsl:template>
  <xsl:template match="SYMBOL">
    <TR>
      <TD><xsl:value-of select="@HITS"/></TD>
      <TD><xsl:value-of select="@NAME"/></TD>
    </TR>
  </xsl:template>
</xsl:transform>
