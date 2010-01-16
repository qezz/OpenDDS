/*
 * $Id$
 *
 * Copyright 2010 Object Computing, Inc.
 *
 * Distributed under the OpenDDS License.
 * See: http://www.opendds.org/license.html
 */

#ifndef DCPSINFO_I_H
#define DCPSINFO_I_H

#include  "inforepo_export.h"
#include /**/ "DCPS_IR_Topic.h"
#include /**/ "DCPS_IR_Topic_Description.h"
#include /**/ "DCPS_IR_Participant.h"
#include /**/ "DCPS_IR_Publication.h"
#include /**/ "DCPS_IR_Subscription.h"
#include /**/ "DCPS_IR_Domain.h"
#include "dds/DCPS/RepoIdGenerator.h"
#include "UpdateManager.h"

#include /**/ "dds/DdsDcpsInfoS.h"
#include /**/ "dds/DdsDcpsDataReaderRemoteC.h"
#include /**/ "dds/DdsDcpsDataWriterRemoteC.h"

#include "tao/ORB_Core.h"

#include <map>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// typedef declarations
typedef std::map<DDS::DomainId_t, DCPS_IR_Domain*> DCPS_IR_Domain_Map;

// Forward declaration
namespace Update {
class Manager;
}

class ShutdownInterface;

/**
 * @class TAO_DDS_DCPSInfo_i
 *
 * @brief Implementation of the DCPSInfo
 *
 * This is the Information Repository object.  Clients of
 * the system will use the CORBA reference of this object.
 */
class  OpenDDS_InfoRepoLib_Export TAO_DDS_DCPSInfo_i
  : public virtual POA_OpenDDS::DCPS::DCPSInfo,
    public ACE_Event_Handler {
public:
  //Constructor
  TAO_DDS_DCPSInfo_i(
    CORBA::ORB_ptr orb,
    bool reincarnate,
    ShutdownInterface* shutdown = 0,
    long federation = 0);

  //Destructor
  virtual ~TAO_DDS_DCPSInfo_i();

  virtual int handle_timeout(const ACE_Time_Value& now,
                             const void* arg);

  virtual CORBA::Boolean attach_participant(
    DDS::DomainId_t domainId,
    const OpenDDS::DCPS::RepoId& participantId)
  ACE_THROW_SPEC((CORBA::SystemException
                   , OpenDDS::DCPS::Invalid_Domain
                   , OpenDDS::DCPS::Invalid_Participant));

  virtual OpenDDS::DCPS::TopicStatus assert_topic(
    OpenDDS::DCPS::RepoId_out topicId,
    DDS::DomainId_t domainId,
    const OpenDDS::DCPS::RepoId& participantId,
    const char * topicName,
    const char * dataTypeName,
    const DDS::TopicQos & qos)
  ACE_THROW_SPEC((CORBA::SystemException
                   , OpenDDS::DCPS::Invalid_Domain
                   , OpenDDS::DCPS::Invalid_Participant));

  /**
   * @brief Add a previously existing topic to the repository.
   *
   * @param topicId       the Topic Entity GUID Id to use.
   * @param domainId      the Domain in which the Topic is contained.
   * @param participantId the Participant in which the Topic is contained.
   * @param topicName     the name of the Topic.
   * @param dataTypeName  the name of the data type.
   * @param qos           the QoS value to use for the Topic.
   *
   * Adds a Topic Entity to the repository using a specified TopicId
   * value.  If the TopicId indicates that this Topic was created by
   * within this repository (the federation Id is the current repositories
   * federation Id), this method will ensure that any subsequent calls to
   * add a Topic and obtain a newly generated Id value will return an Id
   * value greater than the Id value of the current one.
   */
  bool add_topic(const OpenDDS::DCPS::RepoId& topicId,
                 DDS::DomainId_t domainId,
                 const OpenDDS::DCPS::RepoId& participantId,
                 const char* topicName,
                 const char* dataTypeName,
                 const DDS::TopicQos& qos);

  virtual OpenDDS::DCPS::TopicStatus find_topic(
    DDS::DomainId_t domainId,
    const char * topicName,
    CORBA::String_out dataTypeName,
    DDS::TopicQos_out qos,
    OpenDDS::DCPS::RepoId_out topicId)
  ACE_THROW_SPEC((CORBA::SystemException
                   , OpenDDS::DCPS::Invalid_Domain));

  virtual OpenDDS::DCPS::TopicStatus remove_topic(
    DDS::DomainId_t domainId,
    const OpenDDS::DCPS::RepoId& participantId,
    const OpenDDS::DCPS::RepoId& topicId)
  ACE_THROW_SPEC((CORBA::SystemException
                   , OpenDDS::DCPS::Invalid_Domain
                   , OpenDDS::DCPS::Invalid_Participant
                   , OpenDDS::DCPS::Invalid_Topic));

  virtual OpenDDS::DCPS::TopicStatus enable_topic(
    DDS::DomainId_t domainId,
    const OpenDDS::DCPS::RepoId& participantId,
    const OpenDDS::DCPS::RepoId& topicId)
  ACE_THROW_SPEC((CORBA::SystemException
                   , OpenDDS::DCPS::Invalid_Domain
                   , OpenDDS::DCPS::Invalid_Participant
                   , OpenDDS::DCPS::Invalid_Topic));

  virtual OpenDDS::DCPS::RepoId add_publication(
    DDS::DomainId_t domainId,
    const OpenDDS::DCPS::RepoId& participantId,
    const OpenDDS::DCPS::RepoId& topicId,
    OpenDDS::DCPS::DataWriterRemote_ptr publication,
    const DDS::DataWriterQos & qos,
    const OpenDDS::DCPS::TransportInterfaceInfo & transInfo,
    const DDS::PublisherQos & publisherQos)
  ACE_THROW_SPEC((CORBA::SystemException
                   , OpenDDS::DCPS::Invalid_Domain
                   , OpenDDS::DCPS::Invalid_Participant
                   , OpenDDS::DCPS::Invalid_Topic));

  /**
   * @brief Add a previously existing publication to the repository.
   *
   * @param domainId      the Domain in which the Publication is contained.
   * @param participantId the Participant in which the Publication is contained.
   * @param topicId       the Topic of the Publication.
   * @param pubId         the GUID Id value to use for the Publication.
   * @param pub_str       stringified publication callback to DataWriter.
   * @param qos           the QoS value of the DataWriter.
   * @param transInfo     the transport information for the Publication.
   * @param publisherQos  the QoS value of the Publisher.
   * @param associate     indicate whether to create new associations.
   *
   * Adds a Publication to the repository using a specified Publication
   * GUID Id value.  If the PublicationId indicates that this Publication
   * was created by within this repository (the federation Id is the
   * current repositories federation Id), this method will ensure that any
   * subsequent calls to add a Publication and obtain a newly generated
   * Id value will return an Id value greater than the Id value of the
   * current one.
   */
  bool add_publication(DDS::DomainId_t domainId,
                       const OpenDDS::DCPS::RepoId& participantId,
                       const OpenDDS::DCPS::RepoId& topicId,
                       const OpenDDS::DCPS::RepoId& pubId,
                       const char* pub_str,
                       const DDS::DataWriterQos & qos,
                       const OpenDDS::DCPS::TransportInterfaceInfo & transInfo,
                       const DDS::PublisherQos & publisherQos,
                       bool associate = false);

  virtual void remove_publication(
    DDS::DomainId_t domainId,
    const OpenDDS::DCPS::RepoId& participantId,
    const OpenDDS::DCPS::RepoId& publicationId)
  ACE_THROW_SPEC((CORBA::SystemException
                   , OpenDDS::DCPS::Invalid_Domain
                   , OpenDDS::DCPS::Invalid_Participant
                   , OpenDDS::DCPS::Invalid_Publication));

  virtual OpenDDS::DCPS::RepoId add_subscription(
    DDS::DomainId_t domainId,
    const OpenDDS::DCPS::RepoId& participantId,
    const OpenDDS::DCPS::RepoId& topicId,
    OpenDDS::DCPS::DataReaderRemote_ptr subscription,
    const DDS::DataReaderQos & qos,
    const OpenDDS::DCPS::TransportInterfaceInfo & transInfo,
    const DDS::SubscriberQos & subscriberQos)
  ACE_THROW_SPEC((CORBA::SystemException
                   , OpenDDS::DCPS::Invalid_Domain
                   , OpenDDS::DCPS::Invalid_Participant
                   , OpenDDS::DCPS::Invalid_Topic));

  /**
   * @brief Add a previously existing subscription to the repository.
   *
   * @param domainId      the Domain in which the Subscription is contained.
   * @param participantId the Participant in which the Subscription is contained.
   * @param topicId       the Topic of the Subscription.
   * @param subId         the GUID Id value to use for the Subscription.
   * @param sub_str       stringified publication callback to DataReader.
   * @param qos           the QoS value of the DataReader.
   * @param transInfo     the transport information for the Subscription.
   * @param subscriberQos the QoS value of the Subscriber.
   * @param associate     indicate whether to create new associations.
   *
   * Adds a Subscription to the repository using a specified Subscription
   * GUID Id value.  If the SubscriptionId indicates that this Subscription
   * was created by within this repository (the federation Id is the
   * current repositories federation Id), this method will ensure that any
   * subsequent calls to add a Publication and obtain a newly generated
   * Id value will return an Id value greater than the Id value of the
   * current one.
   */
  bool add_subscription(DDS::DomainId_t domainId,
                        const OpenDDS::DCPS::RepoId& participantId,
                        const OpenDDS::DCPS::RepoId& topicId,
                        const OpenDDS::DCPS::RepoId& subId,
                        const char* sub_str,
                        const DDS::DataReaderQos & qos,
                        const OpenDDS::DCPS::TransportInterfaceInfo & transInfo,
                        const DDS::SubscriberQos & subscriberQos,
                        bool associate = false);

  virtual void remove_subscription(
    DDS::DomainId_t domainId,
    const OpenDDS::DCPS::RepoId& participantId,
    const OpenDDS::DCPS::RepoId& subscriptionId)
  ACE_THROW_SPEC((CORBA::SystemException
                   , OpenDDS::DCPS::Invalid_Domain
                   , OpenDDS::DCPS::Invalid_Participant
                   , OpenDDS::DCPS::Invalid_Subscription));

  virtual OpenDDS::DCPS::AddDomainStatus add_domain_participant(
    DDS::DomainId_t domain,
    const DDS::DomainParticipantQos & qos)
  ACE_THROW_SPEC((CORBA::SystemException
                   , OpenDDS::DCPS::Invalid_Domain));

  /**
   * @brief Add a previously existing participant to the repository.
   *
   * @param domainId      the Domain in which the Participant is contained.
   * @param participantId the GUID Id value to use for the Participant.
   * @param qos           the QoS value of the Participant.
   *
   * Adds a Participant to the repository using a specified Participant
   * GUID Id value.  If the ParticipantId indicates that this Participant
   * was created by within this repository (the federation Id is the
   * current repositories federation Id), this method will ensure that any
   * subsequent calls to add a Publication and obtain a newly generated
   * Id value will return an Id value greater than the Id value of the
   * current one.
   */
  bool add_domain_participant(DDS::DomainId_t domainId
                              , const OpenDDS::DCPS::RepoId& participantId
                              , const DDS::DomainParticipantQos & qos);

  virtual void remove_domain_participant(
    DDS::DomainId_t domainId,
    const OpenDDS::DCPS::RepoId& participantId)
  ACE_THROW_SPEC((CORBA::SystemException
                   , OpenDDS::DCPS::Invalid_Domain
                   , OpenDDS::DCPS::Invalid_Participant));

  bool remove_by_owner(
    DDS::DomainId_t domain,
    long              owner);

  virtual void disassociate_participant(
    DDS::DomainId_t domainId,
    const OpenDDS::DCPS::RepoId& local_id,
    const OpenDDS::DCPS::RepoId& remote_id)
  ACE_THROW_SPEC((CORBA::SystemException,
                  OpenDDS::DCPS::Invalid_Domain,
                  OpenDDS::DCPS::Invalid_Participant));

  virtual void disassociate_subscription(
    DDS::DomainId_t domainId,
    const OpenDDS::DCPS::RepoId& participantId,
    const OpenDDS::DCPS::RepoId& local_id,
    const OpenDDS::DCPS::RepoId& remote_id)
  ACE_THROW_SPEC((CORBA::SystemException,
                  OpenDDS::DCPS::Invalid_Domain,
                  OpenDDS::DCPS::Invalid_Participant,
                  OpenDDS::DCPS::Invalid_Subscription));

  virtual void disassociate_publication(
    DDS::DomainId_t domainId,
    const OpenDDS::DCPS::RepoId& participantId,
    const OpenDDS::DCPS::RepoId& local_id,
    const OpenDDS::DCPS::RepoId& remote_id)
  ACE_THROW_SPEC((CORBA::SystemException,
                  OpenDDS::DCPS::Invalid_Domain,
                  OpenDDS::DCPS::Invalid_Participant,
                  OpenDDS::DCPS::Invalid_Publication));

  virtual void ignore_domain_participant(
    DDS::DomainId_t domainId,
    const OpenDDS::DCPS::RepoId& myParticipantId,
    const OpenDDS::DCPS::RepoId& ignoreId)
  ACE_THROW_SPEC((CORBA::SystemException
                   , OpenDDS::DCPS::Invalid_Domain
                   , OpenDDS::DCPS::Invalid_Participant));

  virtual void ignore_topic(
    DDS::DomainId_t domainId,
    const OpenDDS::DCPS::RepoId& myParticipantId,
    const OpenDDS::DCPS::RepoId& ignoreId)
  ACE_THROW_SPEC((CORBA::SystemException
                   , OpenDDS::DCPS::Invalid_Domain
                   , OpenDDS::DCPS::Invalid_Participant
                   , OpenDDS::DCPS::Invalid_Topic));

  virtual void ignore_subscription(
    DDS::DomainId_t domainId,
    const OpenDDS::DCPS::RepoId& myParticipantId,
    const OpenDDS::DCPS::RepoId& ignoreId)
  ACE_THROW_SPEC((CORBA::SystemException
                   , OpenDDS::DCPS::Invalid_Domain
                   , OpenDDS::DCPS::Invalid_Participant
                   , OpenDDS::DCPS::Invalid_Subscription));

  virtual void ignore_publication(
    DDS::DomainId_t domainId,
    const OpenDDS::DCPS::RepoId& myParticipantId,
    const OpenDDS::DCPS::RepoId& ignoreId)
  ACE_THROW_SPEC((CORBA::SystemException
                   , OpenDDS::DCPS::Invalid_Domain
                   , OpenDDS::DCPS::Invalid_Participant
                   , OpenDDS::DCPS::Invalid_Publication));

  virtual CORBA::Boolean update_publication_qos(
    DDS::DomainId_t domainId,
    const OpenDDS::DCPS::RepoId& partId,
    const OpenDDS::DCPS::RepoId& dwId,
    const DDS::DataWriterQos & qos,
    const DDS::PublisherQos & publisherQos)
  ACE_THROW_SPEC((CORBA::SystemException
                   , OpenDDS::DCPS::Invalid_Domain
                   , OpenDDS::DCPS::Invalid_Participant
                   , OpenDDS::DCPS::Invalid_Publication));

  /// Entry for federation updates of DataWriterQos values.
  void update_publication_qos(
    DDS::DomainId_t            domainId,
    const OpenDDS::DCPS::RepoId& partId,
    const OpenDDS::DCPS::RepoId& dwId,
    const DDS::DataWriterQos&  qos);

  /// Entry for federation updates of PublisherQos values.
  void update_publication_qos(
    DDS::DomainId_t            domainId,
    const OpenDDS::DCPS::RepoId& partId,
    const OpenDDS::DCPS::RepoId& dwId,
    const DDS::PublisherQos&   qos);

  virtual CORBA::Boolean update_subscription_qos(
    DDS::DomainId_t domainId,
    const OpenDDS::DCPS::RepoId& partId,
    const OpenDDS::DCPS::RepoId& drId,
    const DDS::DataReaderQos & qos,
    const DDS::SubscriberQos & subscriberQos)
  ACE_THROW_SPEC((CORBA::SystemException
                   , OpenDDS::DCPS::Invalid_Domain
                   , OpenDDS::DCPS::Invalid_Participant
                   , OpenDDS::DCPS::Invalid_Subscription));

  /// Entry for federation updates of DataReaderQos values.
  void update_subscription_qos(
    DDS::DomainId_t            domainId,
    const OpenDDS::DCPS::RepoId& partId,
    const OpenDDS::DCPS::RepoId& drId,
    const DDS::DataReaderQos&  qos);

  /// Entry for federation updates of SubscriberQos values.
  void update_subscription_qos(
    DDS::DomainId_t            domainId,
    const OpenDDS::DCPS::RepoId& partId,
    const OpenDDS::DCPS::RepoId& drId,
    const DDS::SubscriberQos&  qos);

  virtual CORBA::Boolean update_topic_qos(
    const OpenDDS::DCPS::RepoId& topicId,
    DDS::DomainId_t domainId,
    const OpenDDS::DCPS::RepoId& participantId,
    const DDS::TopicQos & qos)
  ACE_THROW_SPEC((CORBA::SystemException
                   , OpenDDS::DCPS::Invalid_Domain
                   , OpenDDS::DCPS::Invalid_Participant
                   , OpenDDS::DCPS::Invalid_Topic));

  virtual CORBA::Boolean update_domain_participant_qos(
    DDS::DomainId_t domain,
    const OpenDDS::DCPS::RepoId& participantId,
    const DDS::DomainParticipantQos & qos)
  ACE_THROW_SPEC((CORBA::SystemException,
                   OpenDDS::DCPS::Invalid_Domain,
                   OpenDDS::DCPS::Invalid_Participant));

  /// Cause the entire repository to exit.
  virtual void shutdown() ACE_THROW_SPEC((CORBA::SystemException));

  /**
   * @brief assert new ownership for a participant and its contained entities.
   *
   * @param domainId      the domain in which the participant resides.
   * @param participantId the participant to be owned.
   * @param sender        the repository sending the update data.
   * @param owner         the repository which is to make callbacks for
   *                      entities within the participant.
   * @return boolean indicating that ownership has been assigned.
   *
   * This establishes @c owner as the new owner of the participant.
   * Ownership consists of calling back to the reader and writer remote
   * interfaces when associations are established and removed from a
   * publication or subscription.  Owner may be the special value of
   * OWNER_NONE to indicate that the previous owner is no longer
   * available to make callbacks and the application has not indicated
   * which repository is to replace it in this capacity.
   *
   * The @c sender of the update is included so that the participant can
   * check that transitions to OWNER_NONE are only honored when initiated
   * by the current owner of the participant.
   *
   * A return value of @c false indicates that the ownership was
   * specified for a domain or participant which could not be found.
   */
  bool changeOwnership(
    DDS::DomainId_t              domainId,
    const OpenDDS::DCPS::RepoId& participantId,
    long                           sender,
    long                           owner);

  /// Initialize the transport for the Built-In Topics
  /// Returns 0 (zero) if succeeds
  int init_transport(int listen_address_given,
                     const ACE_TCHAR* listen_str);

  bool receive_image(const Update::UImage& image);

  /// Add an additional Updater interface.
  void add(Update::Updater* updater);

  /// Convert a domain Id into a reference to a DCPS_IR_Domain object.
  DCPS_IR_Domain* domain(DDS::DomainId_t domain);

  /// Expose a readable reference of the domain map.
  const DCPS_IR_Domain_Map& domains() const;

  /// Expose the ORB.
  CORBA::ORB_ptr orb();

  bool init_persistence();

  bool init_reassociation(const ACE_Time_Value& delay);

private:
  DCPS_IR_Domain_Map domains_;
  CORBA::ORB_var orb_;

  long            federation_;
  RepoIdGenerator participantIdGenerator_;

  Update::Manager* um_;
  bool reincarnate_;

  /// Interface to effect shutdown of the process.
  ShutdownInterface* shutdown_;

  ACE_Recursive_Thread_Mutex lock_;
};

#endif /* DCPSINFO_I_H */
