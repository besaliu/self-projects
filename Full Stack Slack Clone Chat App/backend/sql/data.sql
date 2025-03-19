-- Your data insert statements go here;
TRUNCATE TABLE workspace_channels, channels, user_workspaces, users, workspaces CASCADE;


INSERT INTO channels (id, data) VALUES (
  '06f473bf-83d6-447b-99f4-05d710d1e924',
  jsonb_build_object(
    'name', 'Office Hours',
    'messages', jsonb_build_array(
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', 'd000c343-4d49-4a74-91da-2521757932d2',
        'content', 'This is where we will help students with office hours',
        'timestamp', '2025-01-07T09:30:15Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', 'd000c343-4d49-4a74-91da-2521757932d2',
        'content', 'Ping me for questions!',
        'timestamp', '2025-01-07T09:31:42Z'
      )
    )
  )
);


INSERT INTO channels (id, data) VALUES (
  '00d40958-3fca-44e0-a5ae-feb3a02e4ea4',
  jsonb_build_object(
    'name', 'Fun Staff Day',
    'messages', jsonb_build_array(
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', 'd000c343-4d49-4a74-91da-2521757932d2',
        'content', 'Lets plan a fun staff event!',
        'timestamp', '2025-02-15T11:23:17Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', 'd000c343-4d49-4a74-91da-2521757932d2',
        'content', 'Ping me for questions!',
        'timestamp', '2025-02-15T11:25:08Z'
      )
    )
  )
);

INSERT INTO channels (id, data) VALUES (
  '7a96aec7-7946-4220-8190-6251254bd1c5',
  jsonb_build_object(
    'name', 'Lecture Notes',
    'messages', jsonb_build_array(
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', 'd000c343-4d49-4a74-91da-2521757932d2',
        'content', 'This is where the lectures from last quarter are saved!',
        'timestamp', '2025-01-05T14:12:33Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', 'd000c343-4d49-4a74-91da-2521757932d2',
        'content', 'Ping me for questions!',
        'timestamp', '2025-01-05T14:13:27Z'
      )
    )
  )
);


INSERT INTO channels (id, data) VALUES (
  'c5004a80-3dd7-4349-9326-32e4b603ee2d',
  jsonb_build_object(
    'name', 'TA Intros',
    'messages', jsonb_build_array(
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', 'd000c343-4d49-4a74-91da-2521757932d2',
        'content', 'Welcome to this quarters TAs! Henry and Nicole!',
        'timestamp', '2025-01-03T08:45:19Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', 'd000c343-4d49-4a74-91da-2521757932d2',
        'content', 'Ping me for questions!',
        'timestamp', '2025-01-03T08:47:05Z'
      )
    )
  )
);


INSERT INTO channels (id, data) VALUES (
  'c0c5455a-2ff2-43cf-8aed-4ddee07c84e8',
  jsonb_build_object(
    'name', 'asgn1',
    'messages', jsonb_build_array(
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', '@everyone this is where to ask questions for asgn1 of cse186',
        'timestamp', '2025-01-15T15:32:47Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', 'Has anyone started the assignment yet?',
        'timestamp', '2025-01-15T15:33:21Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', 'd000c343-4d49-4a74-91da-2521757932d2',
        'content', 'Yes, I just finished part 1. Let me know if you have questions!',
        'timestamp', '2025-01-15T16:42:13Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', 'Thanks! Im struggling with the database schema part.',
        'timestamp', '2025-01-15T16:58:40Z'
      )
    )
  )
);

INSERT INTO channels (id, data) VALUES (
  'fa7c0263-6a77-4944-9894-626c14b53d1b',
  jsonb_build_object(
    'name', 'asgn2',
    'messages', jsonb_build_array(
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', '@everyone this is where to ask questions for asgn2 of cse186',
        'timestamp', '2025-02-05T10:15:02Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', 'Has anyone started the assignment yet?',
        'timestamp', '2025-02-05T10:15:58Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', 'd000c343-4d49-4a74-91da-2521757932d2',
        'content', 'Yes, I just finished part 1. Let me know if you have questions!',
        'timestamp', '2025-02-05T11:07:22Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', 'Thanks! Im struggling with the database schema part.',
        'timestamp', '2025-02-05T11:43:35Z'
      )
    )
  )
);

INSERT INTO channels (id, data) VALUES (
  'ae49d018-f8ea-4a25-aa4c-4e0ff373332c',
  jsonb_build_object(
    'name', 'asgn3',
    'messages', jsonb_build_array(
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', '@everyone this is where to ask questions for asgn3 of cse186',
        'timestamp', '2025-03-01T14:23:09Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', 'Has anyone started the assignment yet?',
        'timestamp', '2025-03-01T14:24:37Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', 'd000c343-4d49-4a74-91da-2521757932d2',
        'content', 'Yes, I just finished part 1. Let me know if you have questions!',
        'timestamp', '2025-03-01T15:38:51Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', 'Thanks! Im struggling with the database schema part.',
        'timestamp', '2025-03-01T16:05:27Z'
      )
    )
  )
);

INSERT INTO channels (id, data) VALUES (
  'ab6e4c3e-2668-4219-b324-75729dfb2966',
  jsonb_build_object(
    'name', 'general',
    'messages', jsonb_build_array(
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', '@everyone this is the general chat for cse186',
        'timestamp', '2025-01-04T09:02:45Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', 'Is the next office hours in person or on zoom?',
        'timestamp', '2025-03-10T13:27:16Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', 'd000c343-4d49-4a74-91da-2521757932d2',
        'content', 'I think its on zoom this week!',
        'timestamp', '2025-03-10T13:41:38Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', 'Thanks!',
        'timestamp', '2025-03-10T13:44:12Z'
      )
    )
  )
);

INSERT INTO channels (id, data) VALUES (
  '453d8825-c244-4dd7-b6cb-d54df15412e0',
  jsonb_build_object(
    'name', 'asgn1',
    'messages', jsonb_build_array(
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', '@everyone this is where to ask questions for asgn1 of cse187',
        'timestamp', '2025-01-16T13:22:47Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', 'Has anyone started the assignment yet?',
        'timestamp', '2025-01-16T13:23:21Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', 'd000c343-4d49-4a74-91da-2521757932d2',
        'content', 'Yes, I just finished part 1. Let me know if you have questions!',
        'timestamp', '2025-03-12T15:12:13Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', 'Thanks! Im struggling with the database schema part.',
        'timestamp', '2025-03-13T15:28:40Z'
      )
    )
  )
);

INSERT INTO channels (id, data) VALUES (
  'ca16c14e-b7ee-4c8a-9350-bd54d9ee5645',
  jsonb_build_object(
    'name', 'asgn2',
    'messages', jsonb_build_array(
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', '@everyone this is where to ask questions for asgn2 of cse187',
        'timestamp', '2025-02-07T09:45:02Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', 'Has anyone started the assignment yet?',
        'timestamp', '2025-02-07T09:46:58Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', 'd000c343-4d49-4a74-91da-2521757932d2',
        'content', 'Yes, I just finished part 1. Let me know if you have questions!',
        'timestamp', '2025-02-07T10:17:22Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', 'Thanks! Im struggling with the database schema part.',
        'timestamp', '2025-02-07T10:33:35Z'
      )
    )
  )
);

INSERT INTO channels (id, data) VALUES (
  'efa66ff9-a6c6-47c2-9bbc-4e05aeadefa9',
  jsonb_build_object(
    'name', 'asgn3',
    'messages', jsonb_build_array(
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', '@everyone this is where to ask questions for asgn3 of cse187',
        'timestamp', '2025-03-02T16:13:09Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', 'Has anyone started the assignment yet?',
        'timestamp', '2025-03-02T16:14:37Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', 'd000c343-4d49-4a74-91da-2521757932d2',
        'content', 'Yes, I just finished part 1. Let me know if you have questions!',
        'timestamp', '2025-03-02T17:28:51Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', 'Thanks! Im struggling with the database schema part.',
        'timestamp', '2025-03-02T17:45:27Z'
      )
    )
  )
);

INSERT INTO channels (id, data) VALUES (
  '55a5a4bf-096a-4cb9-9cee-19651dd438a6',
  jsonb_build_object(
    'name', 'general',
    'messages', jsonb_build_array(
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', '@everyone this is the general chat for cse187',
        'timestamp', '2025-01-04T10:32:45Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', 'Is the next office hours in person or on zoom?',
        'timestamp', '2025-03-11T14:17:16Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', 'd000c343-4d49-4a74-91da-2521757932d2',
        'content', 'I think its on zoom this week!',
        'timestamp', '2025-03-11T14:31:38Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', 'Thanks!',
        'timestamp', '2025-03-11T14:34:12Z'
      )
    )
  )
);

INSERT INTO channels (id, data) VALUES (
  '5b01bdb8-64df-43d6-a189-75f848b28132',
  jsonb_build_object(
    'name', 'asgn1',
    'messages', jsonb_build_array(
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', '@everyone this is where to ask questions for asgn1 of cse130',
        'timestamp', '2025-01-16T13:22:47Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', 'Has anyone started the assignment yet?',
        'timestamp', '2025-01-16T13:23:21Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', 'd000c343-4d49-4a74-91da-2521757932d2',
        'content', 'Yes, I just finished part 1. Let me know if you have questions!',
        'timestamp', '2025-03-12T15:12:13Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', 'Thanks! Im struggling with the database schema part.',
        'timestamp', '2025-03-13T15:28:40Z'
      )
    )
  )
);

INSERT INTO channels (id, data) VALUES (
  '2e3cba43-a694-49f4-a975-51c6f7f7a2c4',
  jsonb_build_object(
    'name', 'asgn2',
    'messages', jsonb_build_array(
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', '@everyone this is where to ask questions for asgn2 of cse130',
        'timestamp', '2025-02-06T08:35:02Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', 'Has anyone started the assignment yet?',
        'timestamp', '2025-02-06T08:36:58Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', 'd000c343-4d49-4a74-91da-2521757932d2',
        'content', 'Yes, I just finished part 1. Let me know if you have questions!',
        'timestamp', '2025-02-06T09:27:22Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', 'Thanks! Im struggling with the database schema part.',
        'timestamp', '2025-02-06T09:43:35Z'
      )
    )
  )
);

INSERT INTO channels (id, data) VALUES (
  '20fe38ff-c504-4847-bffa-13ee1b97948c',
  jsonb_build_object(
    'name', 'asgn3',
    'messages', jsonb_build_array(
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', '@everyone this is where to ask questions for asgn3 of cse130',
        'timestamp', '2025-03-03T15:43:09Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', 'Has anyone started the assignment yet?',
        'timestamp', '2025-03-03T15:44:37Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', 'd000c343-4d49-4a74-91da-2521757932d2',
        'content', 'Yes, I just finished part 1. Let me know if you have questions!',
        'timestamp', '2025-03-03T16:58:51Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', 'Thanks! Im struggling with the database schema part.',
        'timestamp', '2025-03-03T17:15:27Z'
      )
    )
  )
);

INSERT INTO channels (id, data) VALUES (
  'a14f33a6-e9e5-4b92-b4f3-9204b515343c',
  jsonb_build_object(
    'name', 'general',
    'messages', jsonb_build_array(
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', '@everyone this is the general chat for cse130',
        'timestamp', '2025-01-04T11:12:45Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', 'Is the next office hours in person or on zoom?',
        'timestamp', '2025-03-09T16:37:16Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', 'd000c343-4d49-4a74-91da-2521757932d2',
        'content', 'I think its on zoom this week!',
        'timestamp', '2025-03-09T16:51:38Z'
      ),
      jsonb_build_object(
        'id', gen_random_uuid(),
        'user_id', '0969fcbb-e184-423d-92a2-b22d3c029875',
        'content', 'Thanks!',
        'timestamp', '2025-03-09T16:54:12Z'
      )
    )
  )
);


INSERT INTO workspaces (id, data) VALUES (
  'bd72e44d-54eb-4946-9b8a-e3bf2fd6ebdb',
  jsonb_build_object('name', 'Staff', 'owner_name', 'Anna Admin')
);

INSERT INTO workspaces (id, data) VALUES (
  'dd2d455d-3ea6-445d-8e3d-b471f6db7e2b',
  jsonb_build_object('name', 'CSE186', 'owner_name', 'Anna Admin')
);

INSERT INTO workspaces (id, data) VALUES (
  'a05acc11-585b-4596-af48-131f454b452d',
  jsonb_build_object('name', 'CSE187', 'owner_name', 'Anna Admin')
);


INSERT INTO workspaces (id, data) VALUES (
  'f5edb470-dd10-4897-a4f4-d1b96c49fa40',
  jsonb_build_object('name', 'CSE130', 'owner_name', 'Molly Member')
);

INSERT INTO workspace_channels (workspace_id, channel_id) VALUES (
 'bd72e44d-54eb-4946-9b8a-e3bf2fd6ebdb', 'c5004a80-3dd7-4349-9326-32e4b603ee2d'
);

INSERT INTO workspace_channels (workspace_id, channel_id) VALUES (
 'bd72e44d-54eb-4946-9b8a-e3bf2fd6ebdb', '00d40958-3fca-44e0-a5ae-feb3a02e4ea4'
);

INSERT INTO workspace_channels (workspace_id, channel_id) VALUES (
 'bd72e44d-54eb-4946-9b8a-e3bf2fd6ebdb', '06f473bf-83d6-447b-99f4-05d710d1e924'
);

INSERT INTO workspace_channels (workspace_id, channel_id) VALUES (
 'bd72e44d-54eb-4946-9b8a-e3bf2fd6ebdb', '7a96aec7-7946-4220-8190-6251254bd1c5'
);

INSERT INTO workspace_channels (workspace_id, channel_id) VALUES (
 'dd2d455d-3ea6-445d-8e3d-b471f6db7e2b', 'c0c5455a-2ff2-43cf-8aed-4ddee07c84e8'
);

INSERT INTO workspace_channels (workspace_id, channel_id) VALUES (
 'dd2d455d-3ea6-445d-8e3d-b471f6db7e2b', 'fa7c0263-6a77-4944-9894-626c14b53d1b'
);

INSERT INTO workspace_channels (workspace_id, channel_id) VALUES (
 'dd2d455d-3ea6-445d-8e3d-b471f6db7e2b', 'ae49d018-f8ea-4a25-aa4c-4e0ff373332c'
);

INSERT INTO workspace_channels (workspace_id, channel_id) VALUES (
 'dd2d455d-3ea6-445d-8e3d-b471f6db7e2b', 'ab6e4c3e-2668-4219-b324-75729dfb2966'
);

INSERT INTO workspace_channels (workspace_id, channel_id) VALUES (
 'a05acc11-585b-4596-af48-131f454b452d', '453d8825-c244-4dd7-b6cb-d54df15412e0'
);

INSERT INTO workspace_channels (workspace_id, channel_id) VALUES (
 'a05acc11-585b-4596-af48-131f454b452d', 'ca16c14e-b7ee-4c8a-9350-bd54d9ee5645'
);


INSERT INTO workspace_channels (workspace_id, channel_id) VALUES (
 'a05acc11-585b-4596-af48-131f454b452d', 'efa66ff9-a6c6-47c2-9bbc-4e05aeadefa9'
);

INSERT INTO workspace_channels (workspace_id, channel_id) VALUES (
 'a05acc11-585b-4596-af48-131f454b452d', '55a5a4bf-096a-4cb9-9cee-19651dd438a6'
);

INSERT INTO workspace_channels (workspace_id, channel_id) VALUES (
 'f5edb470-dd10-4897-a4f4-d1b96c49fa40', '5b01bdb8-64df-43d6-a189-75f848b28132'
);

INSERT INTO workspace_channels (workspace_id, channel_id) VALUES (
 'f5edb470-dd10-4897-a4f4-d1b96c49fa40', '2e3cba43-a694-49f4-a975-51c6f7f7a2c4'
);

INSERT INTO workspace_channels (workspace_id, channel_id) VALUES (
 'f5edb470-dd10-4897-a4f4-d1b96c49fa40', '20fe38ff-c504-4847-bffa-13ee1b97948c'
);

INSERT INTO workspace_channels (workspace_id, channel_id) VALUES (
 'f5edb470-dd10-4897-a4f4-d1b96c49fa40', 'a14f33a6-e9e5-4b92-b4f3-9204b515343c'
);

INSERT INTO users (id, data) VALUES (
  '0969fcbb-e184-423d-92a2-b22d3c029875',
  jsonb_build_object(
    'email', 'molly@books.com',
    'password', '$2a$06$Q9t2Okf13URlYnIpuofZeeMQhfIupHWKFYUGxqjIlL34H5HROxCF2',
    'role', 'member',
    'name', 'Molly Member'
  )
);

INSERT INTO users (id, data) VALUES (
  'd000c343-4d49-4a74-91da-2521757932d2',
  jsonb_build_object(
    'email', 'anna@books.com',
    'password', '$2a$06$0.tOJm1wRK0Yq599gIm3t.6FHxeH/FYlNW8b9ZlPUMG.wHsx3DXLi',
    'role', 'admin',
    'name', 'Anna Admin'
  )
);

INSERT INTO user_workspaces (user_id, workspace_id) VALUES (
'0969fcbb-e184-423d-92a2-b22d3c029875', 'dd2d455d-3ea6-445d-8e3d-b471f6db7e2b'
);
INSERT INTO user_workspaces (user_id, workspace_id) VALUES (
'0969fcbb-e184-423d-92a2-b22d3c029875', 'a05acc11-585b-4596-af48-131f454b452d'
);
INSERT INTO user_workspaces (user_id, workspace_id) VALUES (
'0969fcbb-e184-423d-92a2-b22d3c029875', 'f5edb470-dd10-4897-a4f4-d1b96c49fa40'
);
INSERT INTO user_workspaces (user_id, workspace_id) VALUES (
'd000c343-4d49-4a74-91da-2521757932d2', 'dd2d455d-3ea6-445d-8e3d-b471f6db7e2b'
);
INSERT INTO user_workspaces (user_id, workspace_id) VALUES (
'd000c343-4d49-4a74-91da-2521757932d2', 'a05acc11-585b-4596-af48-131f454b452d'
);
INSERT INTO user_workspaces (user_id, workspace_id) VALUES (
'd000c343-4d49-4a74-91da-2521757932d2', 'f5edb470-dd10-4897-a4f4-d1b96c49fa40'
);

INSERT INTO user_workspaces (user_id, workspace_id) VALUES (
'd000c343-4d49-4a74-91da-2521757932d2', 'bd72e44d-54eb-4946-9b8a-e3bf2fd6ebdb'
);
