using System;

namespace SysCAD.Interface
{
  /// <summary>
  /// Summary description for LineElement.
  /// </summary>
  [Serializable]
  public class Line: Element
  {
    public float x1;
    public float y1;
    public float x2;
    public float y2;

    public Line()
    {
    }

    public Line(float x1, float y1, float x2, float y2)
    {
      this.x1 = x1;
      this.y1 = y1;
      this.x2 = x2;
      this.y2 = y2;
    }

    public override string ToString()
    {
      return "(" + x1.ToString("N2") + ", " + y1.ToString("N2") + ", " + x2.ToString("N2") + ", " + y2.ToString("N2") + ")";
    }
  }
}
