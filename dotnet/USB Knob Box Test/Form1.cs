using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using HidLibrary;

namespace USB_Knob_Box_Test
{
    public partial class Form1 : Form
    {
        private static HidDevice _device;
        public delegate void ReadHandlerDelegate(HidReport report);
        int oldKeys;
        int leds;

        public Form1()
        {
            InitializeComponent();
        }

        private void FormLoad(object sender, EventArgs e)
        {
            // use 0x0014 for EFM8UB2 version; use 0x0015 for PIC18 version
            _device = HidDevices.Enumerate(0x4247, 0x0015).FirstOrDefault();

            if (_device == null)
            {
                MessageBox.Show("Unable to find the USB device.", "Device Not Found", MessageBoxButtons.OK);
            }

            _device.OpenDevice();
            _device.MonitorDeviceEvents = true;
            _device.ReadReport(ReadProcess);

            label1.Text = "0";
            label2.Text = "0";
            label3.Text = "0";
            label4.Text = "0";
            label5.Text = "0";

            oldKeys = 0;
            leds = 0;

            UpdateLeds();
        }

        private void ReadProcess(HidReport report)
        {
            BeginInvoke(new ReadHandlerDelegate(ReadHandler), new object[] { report });
        }

        private void ReadHandler(HidReport report)
        {
            if (report.ReportId == 0x01)
            {
                int keys = report.Data[0];
                int enc1 = report.Data[1];
                int enc2 = report.Data[2];
                int enc3 = report.Data[3];
                int enc4 = report.Data[4];
                int enc5 = report.Data[5];

                if ((enc1 & 1) == 1)
                {
                    if (trackBar1.Value < 10)
                    {
                        trackBar1.Value++;
                        label1.Text = trackBar1.Value.ToString();
                    }
                } else if ((enc1 & 2) == 2)
                {
                    if (trackBar1.Value > 0)
                    {
                        trackBar1.Value--;
                        label1.Text = trackBar1.Value.ToString();
                    }
                }

                if ((enc2 & 1) == 1)
                {
                    if (trackBar2.Value < 10)
                    {
                        trackBar2.Value++;
                        label2.Text = trackBar2.Value.ToString();
                    }
                }
                else if ((enc2 & 2) == 2)
                {
                    if (trackBar2.Value > 0)
                    {
                        trackBar2.Value--;
                        label2.Text = trackBar2.Value.ToString();
                    }
                }

                if ((enc3 & 1) == 1)
                {
                    if (trackBar3.Value < 10)
                    {
                        trackBar3.Value++;
                        label3.Text = trackBar3.Value.ToString();
                    }
                }
                else if ((enc3 & 2) == 2)
                {
                    if (trackBar3.Value > 0)
                    {
                        trackBar3.Value--;
                        label3.Text = trackBar3.Value.ToString();
                    }
                }

                if ((enc4 & 1) == 1)
                {
                    if (trackBar4.Value < 10)
                    {
                        trackBar4.Value++;
                        label4.Text = trackBar4.Value.ToString();
                    }
                }
                else if ((enc4 & 2) == 2)
                {
                    if (trackBar4.Value > 0)
                    {
                        trackBar4.Value--;
                        label4.Text = trackBar4.Value.ToString();
                    }
                }

                if ((enc5 & 1) == 1)
                {
                    if (trackBar5.Value < 100)
                    {
                        trackBar5.Value++;
                        label5.Text = trackBar5.Value.ToString();
                    }
                }
                else if ((enc5 & 2) == 2)
                {
                    if (trackBar5.Value > 0)
                    {
                        trackBar5.Value--;
                        label5.Text = trackBar5.Value.ToString();
                    }
                }

                checkBox1.Checked = checkBox1.Checked ^ (((keys & 1) != 0) && ((oldKeys & 1) == 0));
                checkBox2.Checked = checkBox2.Checked ^ (((keys & 2) != 0) && ((oldKeys & 2) == 0));
                checkBox3.Checked = checkBox3.Checked ^ (((keys & 4) != 0) && ((oldKeys & 4) == 0));
                checkBox4.Checked = checkBox4.Checked ^ (((keys & 8) != 0) && ((oldKeys & 8) == 0));
                checkBox5.Checked = checkBox5.Checked ^ (((keys & 16) != 0) && ((oldKeys & 16) == 0));
                checkBox6.Checked = checkBox6.Checked ^ (((keys & 32) != 0) && ((oldKeys & 32) == 0));
                checkBox7.Checked = checkBox7.Checked ^ (((keys & 64) != 0) && ((oldKeys & 64) == 0));
                checkBox8.Checked = checkBox8.Checked ^ (((keys & 128) != 0) && ((oldKeys & 128) == 0));

                oldKeys = keys;
            }
            _device.ReadReport(ReadProcess);
        }

        private void checkBoxLed1_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBoxLed1.Checked)
            {
                leds |= 0x01;
            }
            else
            {
                leds &= ~0x01;
            }
            UpdateLeds();
        }

        private void checkBoxLed2_CheckedChanged(object sender, EventArgs e)
        {
            if (checkBoxLed2.Checked)
            {
                leds |= 0x02;
            } else
            {
                leds &= ~0x02;
            }
            UpdateLeds();
        }

        private void UpdateLeds ()
        {
            var outData = _device.CreateReport();
            outData.ReportId = 0x02;
            outData.Data[0] = (byte)leds;
            _device.WriteReport(outData);
        }

    }
}
