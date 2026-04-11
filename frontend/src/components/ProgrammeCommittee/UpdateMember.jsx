import React, { useEffect, useState } from 'react';
import axios from 'axios';
import { toast, ToastContainer } from 'react-toastify';
import 'react-toastify/dist/ReactToastify.css';
import { useNavigate, useParams } from 'react-router-dom';
import AdminLoader from '../AdminLoader';

const UpdateProgrammeMember = () => {
  const [image, setImage] = useState(null);
  const [loading, setLoading] = useState(true);
  const { id } = useParams();
  const navigate = useNavigate();

  const [memberData, setMemberData] = useState({
    name: "",
    specialization: "",
    college: "",
    imageUrl: "",
    position: "",
    description: "",
  });

  const parseRoleDescription = (raw) => {
    if (!raw) return { role: '', description: '' };
    const match = String(raw).match(/^Role:\s*(.+?)\s*\n([\s\S]*)$/);
    if (!match) return { role: '', description: String(raw) };
    return { role: match[1], description: match[2] };
  };

  useEffect(() => {
    async function getData() {
      setLoading(true);
      try {
        const res = await axios.get(`${import.meta.env.VITE_API_URL}/api/v1/committee/${id}`, {
          headers: { Authorization: `Bearer ${localStorage.getItem('token')}` },
        });
        const member = res.data.data;
        const parsed = parseRoleDescription(member.description);

        setMemberData({
          college: member.college,
          position: member.position || parsed.role,
          description: parsed.description,
          imageUrl: member.profile_picture_url,
          name: member.name,
          specialization: member.specialization,
        });
      } catch {
        toast.error('Failed to load member details. Please try again.');
      } finally {
        setLoading(false);
      }
    }

    getData();
  }, []);

  const token = localStorage.getItem('token');

  const handleImageChange = (e) => {
    const file = e.target.files[0];
    if (file) {
      setImage(file);
      const imageUrl = URL.createObjectURL(file);
      setMemberData({ ...memberData, imageUrl });
    }
  };

  const handleSubmit = async (e) => {
    e.preventDefault();

    if (!token) {
      toast.error('Please log in first.');
      return;
    }

    try {
      const formData = new FormData();
      if (image) formData.append('file', image);
      formData.append('name', memberData.name);
      formData.append('specialization', memberData.specialization);
      formData.append('college', memberData.college);
      formData.append('committee', 'programme');
      formData.append('description', memberData.description);
      if (memberData.position) formData.append('position', memberData.position);

      const response = await axios.put(
        `${import.meta.env.VITE_API_URL}/api/v1/committee/${id}`,
        formData,
        {
          headers: {
            Authorization: `Bearer ${localStorage.getItem('token')}`,
            'Content-Type': 'multipart/form-data',
          },
        }
      );

      toast.success(response.data.message || 'Committee member updated');
      setImage(null);
      setTimeout(() => {
        navigate('/admin/all-programme-members');
      }, 1000);
    } catch {
      toast.error('Failed to update the committee member. Please try again.');
    }
  };

  return (
    <div className="mx-auto w-full max-w-3xl">
      <div className="admin-card">
        <div className="admin-card-inner">
          <div className="mb-6">
            <h2 className="admin-title">Update Programme Committee Member</h2>
            <p className="admin-muted mt-1">Edit member details and image.</p>
          </div>

          {loading ? (
            <AdminLoader label="Loading member..." />
          ) : (
            <form onSubmit={handleSubmit} className="space-y-4">
              <div>
                <label className="admin-label">Name</label>
                <input
                  type="text"
                  value={memberData.name}
                  onChange={(e) => setMemberData({ ...memberData, name: e.target.value })}
                  className="admin-input"
                  placeholder="Enter member name"
                  required
                />
              </div>

              <div>
                <label className="admin-label">Specialization</label>
                <input
                  type="text"
                  value={memberData.specialization}
                  onChange={(e) => setMemberData({ ...memberData, specialization: e.target.value })}
                  className="admin-input"
                  placeholder="Enter specialization (optional)"
                />
              </div>

              <div>
                <label className="admin-label">College</label>
                <input
                  type="text"
                  value={memberData.college}
                  onChange={(e) => setMemberData({ ...memberData, college: e.target.value })}
                  className="admin-input"
                  placeholder="Enter member's college"
                  required
                />
              </div>

              <div>
                <label className="admin-label">Position</label>
                <input
                  type="text"
                  value={memberData.position}
                  onChange={(e) => setMemberData({ ...memberData, position: e.target.value })}
                  className="admin-input"
                  placeholder="Enter position (optional)"
                />
              </div>

              <div>
                <label className="admin-label">Description</label>
                <input
                  type="text"
                  value={memberData.description}
                  onChange={(e) => setMemberData({ ...memberData, description: e.target.value })}
                  className="admin-input"
                  placeholder="Short bio or description (optional)"
                />
              </div>

              <div>
                <label className="admin-label">Upload Image</label>
                <input
                  type="file"
                  accept="image/*"
                  onChange={handleImageChange}
                  className="mt-2 block w-full text-sm text-zinc-700 file:mr-3 file:rounded-lg file:border-0 file:bg-zinc-100 file:px-4 file:py-2 file:text-sm file:font-medium file:text-zinc-700 hover:file:bg-zinc-200"
                />
                {memberData.imageUrl && (
                  <div className="mt-4">
                    <div className="admin-muted">Preview</div>
                    <img
                      src={memberData.imageUrl}
                      alt="Preview"
                      className="mt-2 w-full rounded-xl border border-zinc-200"
                    />
                  </div>
                )}
              </div>

              <button type="submit" className="admin-button-primary w-full">
                Update Committee Member
              </button>
            </form>
          )}
        </div>
      </div>
      <ToastContainer position="bottom-center" />
    </div>
  );
};

export default UpdateProgrammeMember;
