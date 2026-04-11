import React, { useState } from 'react';
import axios from 'axios';
import { toast, ToastContainer } from 'react-toastify';
import 'react-toastify/dist/ReactToastify.css';

const AddRecentUpdates = () => {
  const [title, setTitle] = useState('');
  const [description, setDescription] = useState('');
  const [link, setLink] = useState('');
  const [type, setType] = useState('recent_update');
  const [priority, setPriority] = useState(0);

  const token = localStorage.getItem('token'); // Get the token from localStorage

  // Handle form submission
  const handleSubmit = async (e) => {
    e.preventDefault();

    if (!token) {
      toast.error('Please log in first.');
      return;
    }

    const updateData = {
      title,
      description,
      type,
      link,
      priority: Number(priority),
    };

    try {
      const response = await axios.post(
        `${import.meta.env.VITE_API_URL}/api/v1/notifications`,
        updateData,
        {
          headers: {
            Authorization: `Bearer ${token}`,
          },
        }
      );

      if (response?.data?.success) {
        toast.success(response.data.message);
        // Reset form after successful submission
        setTitle('');
        setDescription('');
        setLink('');
        setType('recent_update');
        setPriority(0);
      }
    } catch (error) {
      console.error('Error:', error);
      toast.error(error?.response?.data?.message || 'Failed to add update. Please try again.');
    }
  };

  return (
    <div className="mx-auto w-full max-w-3xl">
      <div className="admin-card">
        <div className="admin-card-inner">
          <div className="mb-6">
            <h2 className="admin-title">Add Update</h2>
            <p className="admin-muted mt-1">Post an announcement or important update.</p>
          </div>

          <form onSubmit={handleSubmit} className="space-y-4">
            <div>
              <label className="admin-label">Title</label>
              <input
                type="text"
                value={title}
                onChange={(e) => setTitle(e.target.value)}
                className="admin-input"
                placeholder="Enter update title"
                required
              />
            </div>

            <div>
              <label className="admin-label">Description</label>
              <textarea
                value={description}
                onChange={(e) => setDescription(e.target.value)}
                className="admin-textarea"
                placeholder="Enter update description"
                rows="5"
                required
              />
            </div>

            <div>
              <label className="admin-label">Type</label>
              <select
                value={type}
                onChange={(e) => setType(e.target.value)}
                className="admin-select"
                required
              >
                <option value="highlight">highlight</option>
                <option value="recent_update">recent_update</option>
              </select>
            </div>

            <div>
              <label className="admin-label">Link</label>
              <input
                type="url"
                value={link}
                onChange={(e) => setLink(e.target.value)}
                className="admin-input"
                placeholder="https://..."
                required
              />
            </div>

            <div>
              <label className="admin-label">Priority</label>
              <input
                type="number"
                value={priority}
                onChange={(e) => setPriority(e.target.value)}
                className="admin-input"
                min={0}
              />
            </div>

            <button
              type="submit"
              className="admin-button-primary w-full"
            >
              Add Update
            </button>
          </form>
      </div>
      </div>

      {/* ToastContainer with position set to bottom-center */}
      <ToastContainer position="bottom-center" />
    </div>
  );
};

export default AddRecentUpdates;
