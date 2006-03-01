using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using SysCAD.Interface;

namespace SysCAD.Editor
{
  public partial class OpenProjectForm : Form
  {
    public Config config = new Config();
    public Graphic graphic = new Graphic();

    public OpenProjectForm()
    {
      InitializeComponent();
    }

    private void listProjectsButton_Click(object sender, EventArgs e)
    {
      if (config.Connect(repositoryURLTextBox.Text + "Global"))
      {
        config.Sync();
        projectListBox.Items.Clear();
        foreach (String projectString in config.projectList)
        {
          projectListBox.Items.Add(projectString);
        }

        projectListBox.Enabled = true;
      }
      else
      {
        projectListBox.Items.Clear();
      }
      errorProvider1.SetError(projectListBox, config.connectionError);
    }

    private void projectListBox_SelectedValueChanged(object sender, EventArgs e)
    {
      if (projectListBox.SelectedItem != null)
      {
        if (graphic.Connect(repositoryURLTextBox.Text + projectListBox.SelectedItem.ToString()))
        {
          graphic.Sync();
          openButton.Enabled = true;
        }

        errorProvider1.SetError(projectListBox, graphic.connectionError);
      }
    }

    private void projectListBox_DoubleClick(object sender, EventArgs e)
    {
      openButton.PerformClick();
    }

    private void repositoryURLTextBox_KeyPress(object sender, KeyPressEventArgs e)
    {
      if (e.KeyChar == '\r')
      {
        listProjectsButton.PerformClick();
        e.Handled = true;
      }
    }

    private void projectListBox_KeyPress(object sender, KeyPressEventArgs e)
    {
      if (e.KeyChar == '\r')
      {
        openButton.PerformClick();
      }
      e.Handled = true;
    }
  }
}