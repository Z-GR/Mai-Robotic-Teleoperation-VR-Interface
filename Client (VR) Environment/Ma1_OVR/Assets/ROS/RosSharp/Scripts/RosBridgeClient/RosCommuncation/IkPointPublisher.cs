using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace RosSharp.RosBridgeClient
{
    public class IkPointPublisher : UnityPublisher<MessageTypes.Geometry.Point32>
    {
        public Transform PublishedTransform;

        private MessageTypes.Geometry.Point32 message;

        bool active = false;

        Vector3 rightHandOrigin = new Vector3(0.0f, 0.0f, 0.0f);
        Vector3 rightHandUpdate = new Vector3(0.0f, 0.0f, 0.0f);
        Vector3 rightHandDifferance = new Vector3(0.0f, 0.0f, 0.0f);

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
            message = new MessageTypes.Geometry.Point32();
        }

        private void UpdateMessage()
        {
            OVRInput.Update();

            if (OVRInput.Get(OVRInput.RawButton.RIndexTrigger) && (!active))
            {
                rightHandOrigin = PublishedTransform.position;
                active = true;
                //Debug.Log(rightHandOrigin.x);
            }

            if (OVRInput.Get(OVRInput.RawButton.RIndexTrigger) && (active))
            {
                rightHandUpdate = PublishedTransform.position;
                rightHandDifferance.x = rightHandOrigin.x - rightHandUpdate.x;
                rightHandDifferance.y = rightHandOrigin.y - rightHandUpdate.y;
                rightHandDifferance.z = rightHandOrigin.z - rightHandUpdate.z;

                rightHandDifferance.x = rightHandDifferance.x * 800;
                rightHandDifferance.y = rightHandDifferance.y * 800;
                rightHandDifferance.z = rightHandDifferance.z * 800;
                message.x = -600 + rightHandDifferance.z;
                message.y = -rightHandDifferance.x;
                message.z = -rightHandDifferance.y;
                Debug.Log(rightHandDifferance.x);
            }

            if (!OVRInput.Get(OVRInput.RawButton.RIndexTrigger))
            {
                active = false;
            }

            //Debug.Log(rightHand.y);
            Publish(message);


        }
    }
}