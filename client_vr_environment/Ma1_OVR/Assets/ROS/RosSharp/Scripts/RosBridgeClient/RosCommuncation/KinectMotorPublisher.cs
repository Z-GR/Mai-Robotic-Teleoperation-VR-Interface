using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace RosSharp.RosBridgeClient
{
    public class KinectMotorPublisher : UnityPublisher<MessageTypes.Std.Float64>
    {
        private MessageTypes.Std.Float64 message;

        // Start is called before the first frame update
        protected override void Start()
        {
            base.Start();
            InitializeMessage();
        }

        private void FixedUpdate()
        {
            OVRInput.FixedUpdate();
            UpdateMessage();
        }

        private void InitializeMessage()
        {
            message = new MessageTypes.Std.Float64();
        }

        private void UpdateMessage()
        {
            float x;
            OVRInput.Update();
            if (OVRInput.Get(OVRInput.Button.Four))
            {
                message.data = 0;
                Publish(message);
            }
            if (OVRInput.Get(OVRInput.Button.Three))
            {
                message.data = -30;
                Publish(message);
            }
        }
    }
}
