using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace RosSharp.RosBridgeClient
{
    public class LiftPublisher : UnityPublisher<MessageTypes.Std.Int8>
    {
        private MessageTypes.Std.Int8 message;

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
            message = new MessageTypes.Std.Int8();
        }

        private void UpdateMessage()
        {
            OVRInput.Update();
            if (OVRInput.Get(OVRInput.Button.One))
            {
                message.data = -1;
                Publish(message);
            }
            if (OVRInput.Get(OVRInput.Button.Two))
            {
                message.data = 1;
                Publish(message);
            }
            if (!OVRInput.Get(OVRInput.Button.One) && !OVRInput.Get(OVRInput.Button.Two))
            {
                message.data = 0;
                Publish(message);
            }
        }
    }
}
