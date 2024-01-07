using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace RosSharp.RosBridgeClient
{
    public class GripperPublisher : UnityPublisher<MessageTypes.Std.UInt32>
    {
        private MessageTypes.Std.UInt32 message;

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
            message = new MessageTypes.Std.UInt32();
        }

        private void UpdateMessage()
        {
            OVRInput.Update();
            message.data = System.Convert.ToUInt32(OVRInput.Get(OVRInput.Axis1D.SecondaryHandTrigger, OVRInput.Controller.Touch) * 73);
            Debug.Log(message.data);
            Publish(message);
        }
    }
}
