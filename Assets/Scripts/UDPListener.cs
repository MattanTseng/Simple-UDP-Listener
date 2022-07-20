using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
// Here are the libraries required for the UDP listener. 
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System;


public class UDPListener : MonoBehaviour
{
    //public TextMeshProUGUI Notifications;
    private const int ListenPort= 65013;

    public TMP_Text Switch1Text;
    public TMP_Text Switch2Text;
    public TMP_Text Switch3Text;
    public TMP_Text Pot1Text;
    public TMP_Text Pot2Text;
    public TMP_Text Rot1Text;


    private int RXPort;
    private TMP_Text DisplayText;

    Thread receiveThread;

    private string DisplayMessage;
    public Action NewText;

    public string DataIn;
    // Start is called before the first frame update
    void Start()
    {
        receiveThread = new Thread(
           new ThreadStart(ReceiveData));
        receiveThread.IsBackground = true;
        receiveThread.Start();

        //myDelegate = LeftArrow;

        // assign our delegates to specific functions.
        NewText = ChangeText;

        // Instantiate an instance of our dispatcher class.
        Dispatcher _instance = new Dispatcher();


    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void ReceiveData()
    {
        Debug.Log("Starting to recieve data");
        UdpClient client = new UdpClient(ListenPort);
        while (true)
        {

            print("Test");

            try
            {

                RXPort = ListenPort;
                DisplayText = Switch1Text;
                IPEndPoint anyIP = new IPEndPoint(IPAddress.Any, 0);
                byte[] data = client.Receive(ref anyIP);

                DataIn = Encoding.UTF8.GetString(data);

                print(DataIn);

                DisplayMessage = DataIn;
                Dispatcher.RunOnMainThread(NewText);
                
            }
            catch (SocketException err)
            {
                print(err.ToString());
            }
        }

    }

    private void ChangeText()
    {
        if (DisplayMessage.Contains("S1"))
        {

            Switch1Text.text = DisplayMessage;
        }

        else if (DisplayMessage.Contains("S2"))
        {

            Switch2Text.text = DisplayMessage;
        }
        else if (DisplayMessage.Contains("S3"))
        {

            Switch3Text.text = DisplayMessage;
        }
        else if (DisplayMessage.Contains("P1"))
        {

            Pot1Text.text = DisplayMessage;
        }
        else if (DisplayMessage.Contains("P2"))
        {

            Pot2Text.text = DisplayMessage;
        }
        else if (DisplayMessage.Contains("R1"))
        {

            Rot1Text.text = DisplayMessage;
        }
    }
}
